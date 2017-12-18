/* global Port, guid, LOOP_OBJ */


window._gates = {};


function Gate(type, _in, _out) {
  this.type = type;
  this.in = _in;
  this.out = _out;

  this.uuid = guid();
  this.ports = { };

  // Always 1 extra spot for the color
  let amount_ports = _in + _out + 1;

  // Calculate dimensions
  this.portsPerWidth = Math.ceil(Math.sqrt(amount_ports));
  this.PORT_WIDTH = window.GATE_WIDTH / this.portsPerWidth;

  // Add in-ports
  for (let i=0; i < _in; i++) {
    // Default name
    let name = 'I' + i;
    this.ports[name] = new Port(this, name, this.PORT_WIDTH);
  }

  // Add out-ports
  for (let i=0; i < _out; i++) {
    // Default name
    let name = 'O' + i;
    this.ports[name] = new Port(this, name, this.PORT_WIDTH);
  }

  window._gates[this.uuid] = this;
}


Gate.prototype.setBgColor = function(bgColor) {
  this.bgColor = bgColor;
};


Gate.prototype.getElement = function() {
  if (this.gateElem) return this.gateElem;

  let gateElem = document.createElement('div');

  gateElem.uuid = this.uuid;
  gateElem.className = 'gate gate-' + this.type;

  if (this.bgColor) gateElem.style.backgroundColor = this.bgColor;

  gateElem.draggable = true;
  gateElem.dataset.type = this.type;
  gateElem.dataset.clone = false;


  // Add port elements
  LOOP_OBJ(this.ports).forEach((name, port) => {
    let portElem = port.getElement();

    gateElem.appendChild(portElem);
  });


  this.gateElem = gateElem;
  return gateElem;
};


Gate.prototype.draw = function(circuitElem) {
  let gateElem = this.getElement();
  circuitElem.appendChild(gateElem);
};


Gate.prototype.setPosition = function(x, y) {
  let gateElem = this.getElement();

  // Set position
  gateElem.style.left = x + 'px';
  gateElem.style.top = y + 'px';

  // Set coordinates
  gateElem.x = x;
  gateElem.y = y;

  // Move ports
  LOOP_OBJ(this.ports).forEach((name, port) => {
    let portX = x;
    let portY = y;

    portX += (port.id % this.portsPerWidth) * this.PORT_WIDTH;
    portY += Math.floor(port.id / this.portsPerWidth) * this.PORT_WIDTH;

    port.setPosition(portX, portY);
  });
}


Gate.prototype.destroy = function() {
  // Remove element
  if (this.gateElem) this.gateElem.parentNode.removeChild(this.gateElem);

  // Destroy ports
  LOOP_OBJ(this.ports).forEach((name, port) => {
    port.destroy();
  });

  // Remove from global gates list
  delete window._gates[this.uuid];
};


define(() => Gate);
