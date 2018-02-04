/* global calculateLength, calculateRadians, define */


window._wires = [];


function Wire(a, b) {
    this.a = a;
    this.b = b;

    // TODO: Check if this can be removed
    this.uuid = 'wire#' + a.uuid + '/' + b.uuid;

    window._wires.push(this);
}


Wire.prototype.getElement = function() {
    if (this.wireElem) return this.wireElem;

    let wireElem = document.createElement('div');

    wireElem.uuid = this.uuid;
    wireElem.className = 'wire';

    wireElem.addEventListener('click', this.clicked.bind(this));

    this.wireElem = wireElem;

    this.applyElement();

    return wireElem;
};


Wire.prototype.clicked = function(event) {
    if (event.ctrlKey) {
        this.destroy();
    }
};


Wire.prototype.applyElement = function() {
    // a should always be left from b
    if (this.a.x > this.b.x) {
        // swap a and b
        let _ = this.b;
        this.b = this.a;
        this.a = _;
    }

    // Process gate coordinates
    let ax = this.a.x,
    ay = this.a.y,
    bx = this.b.x,
    by = this.b.y;

    // Check coordinates
    if (isNaN(ax + ay + bx + by)) {
        console.error("Failed to create wire: invalid coordinates", ax, ay, bx, by);
        return;
    }

    // Convert top left to center coordinates
    ax += this.a.port_width / 2;
    ay += this.a.port_width / 2;
    bx += this.b.port_width / 2;
    by += this.b.port_width / 2;


    // Calculate length
    let length = calculateLength(ax, ay, bx, by);

    this.wireElem.style.height = length + 'px';


    // Calculate position
    let x = ax + window.WIRE_WIDTH / 2,
    y = ay + window.WIRE_WIDTH / 2;

    this.wireElem.style.left = x + 'px';
    this.wireElem.style.top = y + 'px';


    // Calculate rotation
    let rotation = calculateRadians(ax, ay, bx, by) - Math.PI / 2;

    this.wireElem.style.transform = 'rotate(' + rotation + 'rad)';
    this.wireElem.style.transformOrigin = 'top left';
}


Wire.prototype.draw = function(circuitElem) {
    circuitElem.appendChild(this.getElement());
};


Wire.prototype.destroy = function() {
    if (this.wireElem) this.wireElem.parentNode.removeChild(this.wireElem);

    // Remove from global wires list
    let uuid = this.uuid;
    // NOTE: `delete` leaves an empty spot in the array, so filter it
    window._wires = window._wires.filter(wire => wire.uuid != uuid);
};


Wire.prototype.redraw = function(circuitElem) {
    if (! this.wireElem) this.getElement();

    this.applyElement();
    this.draw(circuitElem);
};


function redrawAllWires() {
    let circuitElem = document.getElementById('circuit');

    window._wires.forEach(wire => {
        wire.redraw(circuitElem);
    });
}


define(() => Wire);
