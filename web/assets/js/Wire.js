/* global Component, loop, define, Port */


class Wire extends Component {
    static get struct() /*object*/ {
        return {
            uuid: /*uuid*/ 'string',
            from: /*uuid*/ 'string',
            to:   /*uuid*/ 'string',
        };
    }

    static get className() /*string*/ {
        return 'js-wire';
    }


    click(event) /*void*/ {
        console.log(event);

        alert(this.from + " <-> " + this.to + " clicked");
    }


    constructor(/*DOMElement|uuid|Port*/ from, /*DOMElement|uuid|Port*/ to) /*void*/ {
        super();

        // Get UUID from `from`
        if (from instanceof Port) from = from.uuid;
        else if (from instanceof Element) from = from.dataset.uuid;
        if (typeof from !== 'string') throw new TypeError("Couldn't get UUID from `from`");

        // Get UUID from `to`
        if (from instanceof Port) to = to.uuid;
        else if (to instanceof Element) to = to.dataset.uuid;
        if (typeof to !== 'string') throw new TypeError("Couldn't get UUID from `to`");

        this.from = from;
        this.to = to;
    }


    toJSON(/*boolean*/ silent = true) /*object|null*/ {
        /*unused*/ silent;

        return this.forceValid({
            uuid: this.uuid,
            from: this.from,
            to: this.to,
        });
    }


    static fromJSON(/*object*/ json) /*Self*/ {
        this.forceValid(json);

        let wire = new Wire;
        wire.uuid = json.uuid;
        wire.from = json.from;
        wire.to = json.to;
        return wire;
    }


    get element() /*DOMElement*/ {
        let elem = super.element;

        if (elem.jsFresh) {
            elem.addEventListener('click', this.click.bind(this));
        }

        return elem;
    }


    render(/*Component*/ parent) /*DOMElement*/ {
        // Get own DOMElement
        let self = super.render(parent);

        // Attach and render all children
        loop(this.ports).forEach((port) => {
            self.attachElement(port);
            port.render(this);
        });

        self.innerHTML = this.name;

        return self;
    }


    destroy() /*void*/ {
        super.destroy();

        // Destroy all children
        loop(this.ports).forEach((port) => port.destroy());
    }
}


define(() => Wire);
