/* global Component, loop, define */


class Port extends Component {
    static get struct() /*object*/ {
        return {
            uuid: /*uuid*/ 'string',
            type: 'string',
            name: 'string',
        };
    }


    static get className() /*string*/ {
        return 'js-port';
    }


    click(event) /*void*/ {
        console.log(event);

        alert(this.type + " " + this.name + " clicked");
    }


    constructor(/*string*/ type, /*string*/ name) {
        super();

        this.type = type;
        this.name = name;
    }


    toJSON(silent = true) /*object|null*/ {
        if (! this.name && ! silent) this.name = prompt("Name for Port " + this.uuid + " ?");

        if (! this.name) return null;


        return this.forceValid({
            uuid: this.uuid,
            type: this.type,
            name: this.name,
        });
    }


    static fromJSON(/*object*/ json) /*Self*/ {
        this.forceValid(json);

        let port = new Port;
        port.uuid = json.uuid;
        port.type = json.type;
        port.name = json.name;
        return port;
    }


    get element() /*DOMElement*/ {
        let elem = super.element;

        if (elem.jsFresh) {
            elem.dataset.type = this.type;

            elem.addEventListener('click', this.click.bind(this));
        }

        return elem;
    }


    set width(/*int*/ width) /*void*/ {
        let self = this.__element;
        if (! self) throw new TypeError("Can't set the style for port without element");

        // Don't reset the same width
        if (self.style.width === width + 'px') {
            return;
        }

        self.style.width = width + 'px';
        self.style.height = width + 'px';
    }


    render(/*Component*/ parent) /*DOMElement*/ {
        // Get own DOMElement
        let self = super.render(parent);

        if (self.innerHTML !== this.name)
            self.innerHTML = this.name;

        return self;
    }


    destroy() /*void*/ {
        super.destroy();
    }
}


define(() => Port);
