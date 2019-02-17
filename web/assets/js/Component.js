/* global define, guid, validateStruct */

/**
 * Every item on the page should be a component.
 */
class Component {
    constructor() {
        this.__uuid /*uuid*/ = guid();

        // List of all attached children UUID's
        this.__attached /*Array<uuid>*/ = [];
    }

    /**
     * The structure of the class, so that its JSON can get
     * validated. Also see utils.js
     */
    get struct() /*object*/ {
        throw new Error("get struct not implemented");
    }


    get className() /*string*/ {
        throw new Error("get className not implemented");
    }


    /**
     * @param  {Boolean} [silent=true]
     *      If silent is false, it will ask for any missing
     *      information, otherwise (or if it fails),
     *      it returns null.
     */
    toJSON(/*boolean*/ silent = true) /*object|null*/ {
        /*unused*/ silent;

        throw new Error("toJSON not implemented");
    }


    static fromJSON(/*object*/ json) /*Self*/ {
        /*unused*/ json;
        throw new Error("fromJSON not implemented");
    }


    destroy() /*void*/ {
        let self = this.__element;
        if (self) self.parentNode.removeChild(self);
    }


    render(/*Component*/ parent) /*DOMElement*/ {
        /*unused*/parent;

        let self = this.__element;
        if (! self) throw new Error("Can't render without element");
        return self;
    }


    /*- Everything below this line doesn't _need_ to be extended -*/


    get uuid() /*string*/ {
        return this.__uuid;
    }


    clone() /*Self*/ {
        return this.constructor.fromJSON(this.toJSON());
    }


    /**
     * Throw a warning if the json object
     * doesn't match the struct.
     * Return the same object.
     */
    forceValid(/*object*/ json) /*object*/ {
        if (! validateStruct(json, this.constructor.struct)) {
            throw new TypeError("JSON of invalid format");
        }

        return json;
    }


    get element() /*DOMElement*/ {
        if (this.__element) {
            delete this.__element.jsFresh;
            return this.__element;
        }

        let elem = document.createElement('div');
        elem.className = this.constructor.className;
        elem.dataset.uuid = this.uuid;

        // jsFresh is only set if the element is just created
        elem.jsFresh = true;

        return this.__element = elem;
    }


    attachComponent(/*Component*/ other) /*void*/ {
        let self = this.__element;

        if (! self) throw new Error("Can't attach without element");
        if (! other.element) throw new Error("Can't attach invalid element");

        // Don't attach the same element again
        if (this.__attached.includes(other.uuid)) return;

        // Attach DOM element
        self.appendChild(other.element);

        // Append UUID to attached array
        this.__attached.push(other.uuid);
    }


    detachComponent(/*Component*/ other) /*void*/ {
        let self = this.__element;

        if (! self) throw new Error("Can't detach without element");
        if (! other.__element) throw new Error("Can't detach component without element");

        // Don't detach the same element again
        if (! this.__attached.includes(other.uuid)) return;

        // Remove UUID from attached array
        this.__attached.splice(this.__attached.indexOf(other.uuid), 1);

        // Remove DOM element
        self.removeChild(other.__element);
    }
}


define(() => Component);
