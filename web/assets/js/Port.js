/* global redrawAllWires */


window._ports = {};


function Port(gate, name, width) {
  this.name = name;
  this.nameset = false;
  this.gate = gate;

  this.uuid = gate.uuid + ':' + name;
  this.type = (name.charAt(0) == 'I' ? 'IN' : 'OUT');

  this.id = parseInt(name.substring(1)) + (this.type == 'OUT' ? gate.in : 0);

  this.width = width;

  window._ports[this.uuid] = this;
}


Port.prototype.getElement = function() {
  if (this.portElem) return this.portElem;

  let portElem = document.createElement('div');
  portElem.className = 'port port-' + this.type;
  portElem.uuid = this.uuid;

  portElem.port_width = this.width;
  portElem.style.height = portElem.style.width = this.width + 'px';

  portElem.innerHTML = this.name;

  portElem.addEventListener('click', this.click.bind(this));

  this.portElem = portElem;
  return portElem;
};


Port.prototype.setPosition = function(x, y) {
  if (! this.portElem) this.getElement();

  this.portElem.style.left = (x + window.GATE_BORDER) + 'px';
  this.portElem.style.top = (y + window.GATE_BORDER) + 'px';

  this.portElem.x = x;
  this.portElem.y = y;
};


Port.prototype.destroy = function() {
  // Remove element
  if (this.portElem) this.portElem.parentNode.removeChild(this.portElem);

  // Delete from global list
  delete window._ports[this.uuid];

  // Remove wires which connect to this port
  let uuid = this.uuid;

  window._wires = window._wires.filter(wire => {
    // Check if wire connects this element
    if (wire.a.uuid == uuid || wire.b.uuid == uuid) {
      wire.destroy();
      return false;
    }

    return true;
  });

  redrawAllWires();
};


Port.prototype.setName = function(name) {
  let olduuid = this.uuid;

  // Store name
  this.name = name;
  this.uuid = this.gate.uuid + ':' + name;
  this.nameset = true;

  // Update element
  if (this.portElem) this.portElem.innerHTML = name;

  // Update all wires
  window._wires.forEach(wire => {
    if (wire.a.uuid == olduuid) {
      wire.a.uuid = this.uuid;
    }

    if (wire.b.uuid == olduuid) {
      wire.b.uuid = this.uuid;
    }

    wire.uuid = 'wire#' + wire.a.uuid + '/' + wire.b.uuid;
  });
}


Port.prototype.click = function(event) {
  // Make sure ALT is pressed
  if (! event.altKey) return;

  // Get name
  let name = prompt("Enter name for port");
  if (! name) return;

  // Set name
  this.setName(name);
}


define(() => Port);
