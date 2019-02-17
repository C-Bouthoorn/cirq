/* global LOOP_OBJ, define */


function Circuit(gates, wires) {
    this.gates = gates;
    this.wires = wires;
}


Circuit.prototype.generateJSON = function() {
    let output = {
        gates: {},
        wires: [],
    };

    // Add gates to output
    LOOP_OBJ(this.gates).forEach((uuid, gate) => {
        output.gates[uuid] = {
            // Only store the type and the ports
            type: gate.type,
            ports: LOOP_OBJ(gate.ports).values()
                            .map(port => port.toJSON()),
        };
    });


    // Add wires to output
    this.wires.forEach(wire => {
        output.wires.push({
            a: wire.a.uuid,
            b: wire.b.uuid,
        });
    });


    return output;
};


define(() => Circuit);
