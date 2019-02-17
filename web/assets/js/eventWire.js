window.wireFrom = null;


function mousedown(event) /*void*/ {
    // Shift has to be pressed
    if (! event.shiftKey) return;

    window.wireFrom = event.target;
}


function mouseup(event) /*void*/ {
    // Can't add wire without from target
    if (! window.wireFrom) return;

    // Check if targets are valid
    let wireFrom = window.wireFrom;
    let wireTo = event.target;

    if (! isValidTarget(wireFrom) || ! isValidTarget(wireTo)) {
        throw new TypeError("Invalid destinations for wire");
    }

    // Create wire
    window.circuit.addWire(wireFrom, wireTo);
}


function isValidTarget(/*DOMElement*/ elem) {
    return elem.classList.contains('js-port');
}



document.addEventListener('mousedown', mousedown, false);
document.addEventListener('mouseup', mouseup, false);
