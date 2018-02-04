/* globals Circuit, CustomGate, LOOP_OBJ, CLONE, Gate, Wire */

window._customGates = [];


function getAllIONamesFromGates(gates) {
    let failed = false;
    let ionames = {};


    LOOP_OBJ(gates).forEach((uuid, gate) => {
        if (failed) return;

        if (gate.type == 'IN' || gate.type == 'OUT') {
            // get port
            let port = gate.ports['I0'] || gate.ports['O0'];

            // check if name is set
            if (! port.nameset) {
                alert("Not all in- and outputs have names.");
                failed = true;
                return;
            }

            // add port name to ionames
            ionames[uuid] = port.name;
        }
    });


    if (failed) return false;
    return ionames;
}


function createCustomGateFromCurrentCircuit() {
    // Create new circuit
    let circuit = new Circuit(CLONE(window._gates), CLONE(window._wires));

    // Make sure all in- and outputs have names
    let ionames = getAllIONamesFromGates(window._gates);
    if (ionames === false) return;

    // Get missing information
    let type = prompt("Type?");
    if (! type) return false;

    let bgcolor = prompt("Color?");
    if (! bgcolor) return false;

    // Create new gate
    let newgate = new CustomGate(type, ionames, circuit);
    newgate.bgcolor = bgcolor;

    window._customGates[type] = newgate;
    console.log(newgate);

    // Create element
    let gateElem = newgate.getElement();
    document.querySelector('.toolbox .box.custom').appendChild(gateElem);
}


function addInfoToGates() {
    let gates = document.querySelectorAll('.toolbox .gate');

    gates.forEach(gate => {
        gate.innerHTML = window.GATE_INNER_TEMPLATE.apply({
            type: gate.dataset.type,
            in: gate.dataset.in,
            out: gate.dataset.out,
        });
    });
}

addInfoToGates();


function clearCircuit() {
    // Destroy all wires
    window._wires.forEach(wire => {
        wire.destroy();
    });

    // Destroy all gates
    LOOP_OBJ(window._gates).forEach((name, gate) => {
        gate.destroy();
    });


    // Check that everything is gone
    let circuit = document.getElementById('circuit');

    if (circuit.children.length > 0) {
        console.log("circuit not empty?");
        console.log(circuit.children);

        // force remove child
        circuit.children.forEach(child => circuit.removeChild(child));
    }

    if (LOOP_OBJ(window._gates).length() > 0) {
        console.log("_gates not empty?");
        console.log(window._gates);

        // force remove gates
        window._gates = {};
    }

    if (window._wires > 0) {
        console.log("_wires not empty?");
        console.log(window._wires);

        // force remove wires
        window._wires = [];
    }
}


function generateJSON() {
    // Create current circuit
    let circuit = new Circuit(CLONE(window._gates), CLONE(window._wires));

    // Make sure all IO ports have names
    if (getAllIONamesFromGates(window._gates) === false) return;

    // Get custom gates
    let customGates = window._customGates;
    customGates = LOOP_OBJ(customGates)
        // convert inner circuits to json
        .map((_, customGate) => customGate.circuit.generateJSON())
        .removeLoop();

    let name = prompt("Enter a name for this circuit: ");
    if (! name) return false;

    return {
        name: name,
        circuit: circuit.generateJSON(),
        customGates: customGates,
    };
}


function generateFile(json) {
    let output = "";

    if (json === false) return false;

    /*
    full_adder

    [gates] 8
    0e36e9ea IN #I0
    63f0d9e3 IN #I1
    b9f8820d IN #Ci
    2c9ced3a OUT #S
    57f26486 OUT #Co
    e16e5e97 OR
    3752723b half_adder
    45243a79 half_adder

    [wires] 8
    0e36e9ea:I0 3752723b:I0
    63f0d9e3:I1 3752723b:I1
    3752723b:S 45243a79:I0
    b9f8820d:Ci 45243a79:I1
    45243a79:S 2c9ced3a:S
    3752723b:C e16e5e97:I0
    45243a79:C e16e5e97:I1
    e16e5e97:O0 57f26486:Co
    */

    output += json.name + "\n";

    output += "\n";

    // [gates] 8
    output += "[gates] " + LOOP_OBJ(json.circuit.gates).length() + "\n";


    LOOP_OBJ(json.circuit.gates).forEach(
        (uuid, gate) => {
            // 0e36e9ea IN
            output += uuid + " " + gate.type;

            //  #I0
            if (gate.type == "IN" || gate.type == "OUT") {
                output += " #" + (gate.ports['O0'] || gate.ports['I0']).name;
            }

            output += "\n";
        }
    );

    output += "\n";

    // [wires] 8
    output += "[wires] " + LOOP_OBJ(json.circuit.wires).length() + "\n";

    json.circuit.wires.forEach(wire => {
        // 0e36e9ea:I0 3752723b:I0
        output += wire.a + " " + wire.b + "\n";
    });

    output += "\n";

    // Repeat the same function for all custom gates
    if (json.customGates) {
        LOOP_OBJ(json.customGates).forEach((name, circuit) => {
            output += generateFile({
                name: name,
                circuit: circuit,
            });

            output += "\n";
        });
    }

    return output;
}


function loadCustomGateJSON(json) {
    // Make sure `json` is a JSON Object
    if (typeof json.name === 'undefined') {
        json = JSON.parse(json);
    }

    /*
        CustomGate(type, ionames, circuit)
        Gate(type, _in, _out)
        Wire(a, b)
    */

    let gates = LOOP_OBJ(json.circuit.gates).map(
        (uuid, data) => {
            let _in = 0, _out = 0;

            // Get amount of inports and outports
            LOOP_OBJ(data.ports).forEach((portuuid) => {
                if (portuuid.charAt(0) == 'O') _in++;
                else _out++;
            });

            let gate = new Gate(data.type, _in, _out);

            return gate;
        }
    ).removeLoop();

    console.log(gates);


    let ionames = getAllIONamesFromGates(gates);

    let wires = json.circuit.wires.map(
        (data) => {
            let wire = new Wire(gates[data.a], gates[data.b]);

            return wire;
        }
    );

    let circuit = new Circuit(gates, wires);

    let custom = new CustomGate(json.name, ionames, circuit);

    custom.bgcolor = prompt("Color?");


    window._customGates[json.name] = custom;
    console.log(custom);

    // Create element
    let gateElem = custom.getElement();
    document.querySelector('.toolbox .box.custom').appendChild(gateElem);
}
