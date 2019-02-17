/* global define, loop, Component, Gate, Wire */

class Circuit extends Component {
    static get struct() /*object*/ {
        return {
            uuid: /*uuid*/ 'string',
            name: 'string',
            gates: ['object', Gate.struct],
            wires: ['object', Wire.struct],
        }
    }

    static get className() /*string*/ {
        return 'js-circuit';
    }


    deleteGate(/*uuid*/ gateuuid) /*void*/ {
        let gate = this.gates[gateuuid];

        // Detach and destroy gate
        this.detachComponent(gate);
        gate.destroy();

        // Remove from gates
        delete this.gates[gateuuid];
    }


    addGate(/*string*/ gatetype, /*number*/ inports, /*number*/ outports) /*uuid*/ {
        let gate = new Gate(gatetype);

        gate.setPorts(inports, outports);

        this.gates[gate.uuid] = gate;

        this.attachComponent(gate);

        return gate.uuid;
    }


    moveGate(/*uuid*/ gateuuid, event) /*void*/ {
        let gate = this.gates[gateuuid];
        gate.move(event);
    }


    addWire(/*DOMElement|uuid|Port*/ from, /*DOMElement|uuid|Port*/ to) /*Wire*/ {
        let wire = new Wire(from, to);

        this.wires[wire.uuid] = wire;

        return wire;
    }


    constructor() {
        super();

        this.gates = {};
        this.wires = {};
    }


    toJSON(silent = true) /*object|null*/ {
        if (! this.name && ! silent) this.name = prompt("Name for circuit " + this.uuid + " ?");

        if (! this.name) return null;

        return this.forceValid({
            uuid: this.uuid,
            name: this.name,
            gates: loop(this.gates).map((gate) => gate.toJSON(silent)).obj(),
            wires: loop(this.wires).map((wire) => wire.toJSON(silent)).obj(),
        });
    }


    static fromJSON(/*object*/ json) /*Self*/ {
        // Make sure JSON is valid
        this.forceValid(json);

        // Create new object and set fields
        let circ = new Circuit;
        circ.uuid = json.uuid;
        circ.gates = loop(json.gates).map((gatejson) => Gate.fromJSON(gatejson)).obj();
        circ.wires = loop(json.wires).map((wirejson) => Wire.fromJSON(wirejson)).obj();

        return circ;
    }


    render(/*Component*/ parent) /*DOMElement*/ {
        // Get own DOMElement
        let self = super.render(parent);

        // Attach and render all children
        loop(this.gates).forEach((gate) => {
            this.attachComponent(gate);
            gate.render(this);
        });

        loop(this.wires).forEach((wire) => {
            this.attachComponent(wire);
            wire.render(this);
        });

        return self;
    }


    destroy() /*void*/ {
        super.destroy();

        // Detach and destroy all children
        loop(this.gates).forEach((gate) => {
            this.detachComponent(gate);
            gate.destroy();
        });

        loop(this.wires).forEach((wire) => {
            this.detachComponent(wire);
            wire.destroy();
        });

        delete this.__element;
    }
}


define(() => Circuit);
