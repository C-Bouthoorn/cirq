/* global Gate, Port, redrawAllWires, LOOP_OBJ */


let dragged = null;


function dragstart(event) {
    // element is picked up
    dragged = event.target;

    if (dragged.draggable && ! event.shiftKey) {
        // Otherwise Firefox won't drag this object
        event.dataTransfer.setData('text/plain', 'Yes, I am dragging');
    }
    else {
        event.preventDefault();
    }
}


function dragend(event) {
    // element is released
    dragged = null;

    // unused
    event;
}


function dragover(event) {
    // element enters new zone

    if (event.target.id == 'circuit' || event.target.classList.contains('trash')) {
        // element may be dropped here
        event.preventDefault();
    }
}


function dragenter(event) {
    // element enters droppable zone

    // unused
    event;
}


function dragleave(event) {
    // element leaves droppable zone

    // unused
    event;
}


function drop(event) {
    // element gets dropped
    event.preventDefault();

    // If element is dropped in the trash
    if (event.target.classList.contains('trash')) {
        // Custom gate in the toolbox
        if (dragged.classList.contains('gate-custom') && dragged.dataset.clone == "true") {
            let type = dragged.dataset.type;

            if (confirm("Delete " + type + " ?")) {
                // Delete custom gate
                dragged.parentNode.removeChild(dragged);

                delete window._customGates[type];

                dragged = null;
            }

            return;
        }

        // Don't remove items in the toolbox
        if (dragged.parentNode.classList.contains('box')) {
            return;
        }

        // Destroy gate
        let gate = window._gates[dragged.uuid];
        if (gate) {
            // If it's a known gate: remove it the usual way
            gate.destroy();
        }
        else {
            // Unknown gate?
            console.log("Deleting unknown gate?");
            dragged.parentNode.removeChild(dragged);
        }

        return;
    }


    let gate;

    if (dragged.dataset.clone == "true") {
        // Clone dragged gate
        let type = dragged.dataset.type;

        gate = new Gate(type, parseInt(dragged.dataset.in), parseInt(dragged.dataset.out));

        // If it's a custom gate
        if (dragged.classList.contains('gate-custom')) {
            // Set new background color
            gate.setBgColor(dragged.style.backgroundColor);

            // Rename ports to the names of the custom gate ports
            let customGate = window._customGates[type];
            let ionames = customGate.ionames;
            let ins = LOOP_OBJ(customGate.circuit.gates).filter((uuid, gate) => gate.type == 'IN').values();
            let outs = LOOP_OBJ(customGate.circuit.gates).filter((uuid, gate) => gate.type == 'OUT').values();


            LOOP_OBJ(gate.ports).forEach((name, port) => {
                if (port.type == Port.TYPE_ENUM.IN) {
                    port.setName(ionames[ins.pop().uuid]);
                }
                else if (port.type == Port.TYPE_ENUM.OUT) {
                    port.setName(ionames[outs.pop().uuid]);
                }
                else {
                    console.error("Ehm, help? port.type ==", port.type);
                }
            });
        }
    }
    else {
        // Re-use old gate
        gate = window._gates[dragged.uuid];
    }


    // Calculate position of gate
    let x = event.clientX;
    let y = event.clientY;

    // Convert top left to center
    x -= window.GATE_WIDTH / 2;
    y -= window.GATE_WIDTH / 2;

    gate.setPosition(x, y);

    // Draw gate to circuit
    let circuitElem = event.target;
    gate.draw(circuitElem);

    // Redraw all wires
    redrawAllWires();
}


document.addEventListener('dragstart', dragstart, false);
document.addEventListener('dragend', dragend, false);
document.addEventListener('dragover', dragover, false);
document.addEventListener('dragenter', dragenter, false);
document.addEventListener('dragleave', dragleave, false);
document.addEventListener('drop', drop, false);
