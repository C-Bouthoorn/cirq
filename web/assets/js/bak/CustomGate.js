/* globals LOOP_OBJ, define, Template, HTMLToElem */

const CUSTOM_GATE_TEMPLATE = new Template(
    '<div' +
    ' class="gate gate-custom gate-{{TYPE}}"'+
    ' style="background-color: {{BGCOLOR}}"' +
    ' draggable="true"' +
    ' data-clone="true"' +
    ' data-type="{{TYPE}}"' +
    ' data-in="{{IN}}"' +
    ' data-out="{{OUT}}">' +
    window.GATE_INNER_TEMPLATE.html +
    '</div>'
);


function CustomGate(type, ionames, circuit) {
    this.type = type;
    this.ionames = ionames;
    this.circuit = circuit;
}


CustomGate.prototype.in = function() {
    let _in = 0;

    LOOP_OBJ(this.circuit.gates).forEach((name, gate) => {
        if (gate.type == 'IN') _in++;
    });

    return _in;
};


CustomGate.prototype.out = function() {
    let _out = 0;

    LOOP_OBJ(this.circuit.gates).forEach((name, gate) => {
        if (gate.type == 'OUT') _out++;
    });

    return _out;
};


CustomGate.prototype.getElement = function() {
    if (this.gateElem) return this.gateElem;

    let gateHTML = CUSTOM_GATE_TEMPLATE.apply({
        type: this.type,
        bgcolor: this.bgcolor,
        in: this.in(),
        out: this.out(),
    });

    let gateElem = HTMLToElem(gateHTML);

    this.gateElem = gateElem;
    return gateElem;
};


define(() => CustomGate);
