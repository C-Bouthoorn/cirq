/* globals Circuit, loop */

// The circuit that's currently displayed
window.circuit /*Circuit*/ = createCircuit();

// Any additional circuits that are added to the toolbox
window.customCircuits /*Array<Circuit>*/ = [];


/**
 * Create a new circuit and add it to the DOM
 */
function createCircuit() /*Circuit*/ {
    let circuit = new Circuit;

    // Add circuit to DOM
    document.querySelector('#circuit').appendChild(circuit.element);

    return circuit;
}


/**
 * Save the currently displayed gate into the toolbox, then
 * create and render a new circuit.
 */
function createCustomGate() /*void*/ {
    // Debug gate
    console.log(window.circuit);

    // Destroy all DOM elements since it doesn't have to be rendered anymore
    window.circuit.destroy();

    // Save circuit in toolbox
    window.customCircuits.push(window.circuit);

    // Create new circuit
    window.circuit = createCircuit();
}
/*export*/ createCustomGate;


function generateJSON() /*object*/ {
    let json = {
        circuit: window.circuit.toJSON(false),
        customCircuits: window.customCircuits.map((circ) => circ.toJSON(false)),
    };

    console.log(json);
    return json;
}
/*export*/ generateJSON;


function fromJSON(/*string|object*/ json) /*Circuit*/ {
    // If `json` is a string, it probably still needs to be parsed
    if (typeof json === 'string') {
        json = JSON.parse(json);
    }

    if (loop(window.circuit.gates).length() > 0) {
        alert("The current circuit isn't empty - save and clear that one first.");
        return false;
    }

    return window.circuit = Circuit.fromJSON(json);
}
/*export*/ fromJSON;


function clearCircuit() /*void*/ {
    if (! confirm("Clear circuit?")) {
        return;
    }

    window.circuit = new Circuit;
}
/*export*/ clearCircuit;


function generateFile() /*string*/ {
    let output = "";
    let json = generateJSON();

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

   output += json.circuit.name + "\n\n";

   output += "[gates] " + loop(json.circuit.gates).length() + "\n";

   loop(json.circuit.gates).matchall((gate) => {
       output += gate.uuid + " " + gate.type;

       if (gate.type === 'IN' || gate.type === 'OUT') {
           output += " #" + gate.ports[0].name;
       }
   });

   output += "\n[wires] " + loop(json.circuit.wires).length() + "\n";

   // @TODO: Add wires


   console.log(output);
   return output;
}
/*export*/ generateFile;



setInterval(() => window.circuit.render(), 1000);
