/* global Component, loop, define, Port */


class Gate extends Component {
    static get struct() {
        return {
            uuid: /*uuid*/ 'string',
            type: 'string',
            color: 'string',
            ports: ['object', Port.struct],
        };
    }

    static get className() {
        return 'js-gate';
    }

    static get WIDTH() { return 100; }
    static get HEIGHT() { return 100; }


    /**
     * Move to the location of the given event.
     */
    move(event) {
        if (! this.__element) throw new Error("Can't move without element");

        let x = event.clientX - this.constructor.WIDTH / 2;
        let y = event.clientY - this.constructor.HEIGHT / 2;

        this.element.style.left = x + 'px';
        this.element.style.top = y + 'px';
    }


    setPorts(/*number*/ inports, /*number*/ outports) /*void*/ {
        // Make sure there are no ports already set
        if (loop(this.ports).length() !== 0) {
            throw new Error("Can't overwrite ports");
        }

        // Add `inports` in-ports
        for (let i=0; i < inports; i++) {
            let port = new Port('in', 'I' + i);
            this.ports[port.uuid] = port;
        }

        // Add `outports` out-ports
        for (let i=0; i < outports; i++) {
            let port = new Port('out', 'O' + i);
            this.ports[port.uuid] = port;
        }
    }


    constructor(/*string*/ type) {
        super();

        this.type = type;
        this.ports = {};
    }


    toJSON(silent = true) /*object|null*/ {
        if (! this.color && ! silent) this.color = prompt("Color for Gate " + this.uuid + " ?");

        if (! this.color) return null;

        return this.forceValid({
            uuid: this.uuid,
            type: this.type,
            color: this.color,
            ports: loop(this.ports).map((port) => port.toJSON(silent)).obj(),
        });
    }


    static fromJSON(/*object*/ json) /*Self*/ {
        this.forceValid(json);

        let gate = new Gate;
        gate.uuid = json.uuid;
        gate.type = json.type;
        gate.color = json.color;
        gate.ports = loop(json.ports).map((portjson) => Port.fromJSON(portjson)).obj();
        return gate;
    }


    get element() /*DOMElement*/ {
        let elem = super.element;

        if (elem.jsFresh) {
            elem.draggable = true;

            elem.dataset.type = this.type;

            elem.style.backgroundColor = this.color;
        }

        return elem;
    }


    render(/*Component*/ parent) /*DOMElement*/ {
        // Get own DOMElement
        let self = super.render(parent);

        // Determine width of the ports
        let portsPerWidth = Math.ceil(Math.sqrt(
            loop(this.ports).length() + 1 /* for the remaining colour*/
        ));
        let portWidth = Gate.WIDTH / portsPerWidth;

        // Attach and render all children
        loop(this.ports).forEach((port) => {
            this.attachComponent(port);
            port.render(this);
            port.width = portWidth;
        });

        return self;
    }


    destroy() /*void*/ {
        console.log("destroy", this);

        super.destroy();

        // Destroy all children
        loop(this.ports).forEach((port) => {
            this.detachComponent(port);
            port.destroy();
        });
    }
}


define(() => Gate);
