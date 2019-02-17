// The element that's currently being dragged
window.$dragging /*DOMElement*/ = null;


function dragstart(event) /*void*/ {
    // Keep track of what we're dragging
    window.$dragging = event.target;

    // Firefox won't drag items if there is no data set, but we're using a global
    // to take care of the dragging, so just set some foo data.
    event.dataTransfer.setData('text/plain', 'See the window.$dragging element');
}


function dragend(event) /*void*/ {
    /*unused*/ event;

    window.$dragging = null;
}


function dragover(event) /*void*/ {
    // Target may only be dropped onto the circuit or in the trash
    if (event.target.classList.contains('js-circuit') || event.target.classList.contains('trash')) {
        event.preventDefault();
    }
}


function drop(event) /*void*/ {
    console.log('drop', event);

    // Always allow dropping
    event.preventDefault();

    // Check current state of the draggable item
    let intoTrash = event.target.classList.contains('js-trash');
    let inCircuit = window.$dragging.parentNode.classList.contains('js-circuit');

    if (intoTrash) {
        if (inCircuit)
            trashGate(window.$dragging);
        else
            deleteCustomGate(window.$dragging);
    }
    else {
        if (inCircuit)
            moveGate(window.$dragging, event);
        else
            addGate(window.$dragging, event);
    }
}


function trashGate(/*DOMElement*/ $gate) /*void*/ {
    window.circuit.deleteGate($gate.dataset.uuid);
}


function deleteCustomGate(/*DOMElement*/ $gate) /*void*/ {
    let uuid = $gate.dataset.uuid;
    let gate = window.customGates[uuid];

    // Ask for confirmation
    if (! confirm("Delete custom gate " + gate.name + " ?")) {
        return;
    }

    // Delete from custom gates
    delete window.customGates[uuid];
}


function moveGate(/*DOMElement*/ $gate, event) /*void*/ {
    let uuid = $gate.dataset.uuid;
    window.circuit.moveGate(uuid, event);
}


function addGate(/*DOMElement*/ $gate, event) /*void*/ {
    let uuid = window.circuit.addGate($gate.dataset.type, $gate.dataset.in, $gate.dataset.out);
    window.circuit.moveGate(uuid, event);
}



document.addEventListener('dragend', dragend, false);
document.addEventListener('dragstart', dragstart, false);
document.addEventListener('dragover', dragover, false);
document.addEventListener('drop', drop, false);
