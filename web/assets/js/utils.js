// Why isn't this the default?
Object.prototype.values = function() {
    return Object.values(this);
};
Object.prototype.keys = function() {
    return Object.keys(this);
};



/**
 * Generate a pseudo-random string of 8 characters.
 *
 * Based on https://stackoverflow.com/a/105074/4141219
 * @return string
 */
function guid() {
    function s4() {
        return Math.floor((Math.random() + 1) * 0x10000)
        .toString(16)
        .substring(1);
    }

    return s4() + s4();
}
/*export*/ guid;


/**
 * Merge several objects into one single object
 * @return object   The merged object
 */
function mergeObjs(/*object...*/) /*object*/ {
    let res = {};

    Array.from(arguments).forEach((arg) => {
        for (let k in arg) {
            if (typeof res[k] !== 'undefined') {
                throw new TypeError("Duplicate key '" + k + "'");
            }

            res[k] = arg[k];
        }
    });

    return res;
}
/*export*/ mergeObjs;



/**
 * Loop over a regular object as if it were an array,
 * except the callback gets the value and the key.
 */
function loop(obj) {
    // Don't add another wrapping layer around it
    if (obj.__looped) return obj;


    return Object.assign({}, obj, {
        __looped: true,


        forEach: function(callback) {
            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                callback(v, k);
            }
        },


        map: function(callback) {
            let results = {};

            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                if (typeof (results[k] = callback(v, k)) === 'undefined') {
                    console.warn("Please consider using forEach instead of map if you're not returning anything");
                }
            }

            return loop(results);
        },


        filter: function(callback) {
            let results = {};

            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                if (callback(v, k)) {
                    results[k] = v;
                }
            }

            return loop(results);
        },


        // Check if for all entries the callback returns true
        matchall: function(callback) {
            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                if (! callback(v, k)) {
                    return false;
                }
            }

            return true;
        },


        keys: function() {
            let keys = [];

            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                keys.push(k);
            }

            return keys;
        },


        values: function() {
            let values = [];

            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                values.push(obj[k]);
            }

            return values;
        },


        obj: function() {
            return obj;
        },


        length: function() {
            let len = 0;

            for (let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                len++;
            }

            return len;
        },
    });
}
/*export*/ loop;


/**
 * Validate that a JSON object matches a given structure.
 * If it should be matched strictly, no fields may be
 * extra or missing.
 */
function validateStruct(json, struct, strict=false) /*boolean*/ {
    // struct can be the typeof of the object.
    if (typeof struct === 'string') {
        if (struct === 'array') {
            return Array.isArray(json);
        }

        return (typeof json === struct);
    }

    // struct can also be a custom validator function,
    // which we should just call with the data
    if (typeof struct === 'function') {
        return struct(json, strict);
    }

    // Check if `json` has as many fields as `struct` if strict
    if (strict) {
        if (loop(json).length() !== loop(struct).length()) {
            return false;
        }
    }

    // Check if all fields of the struct match the json
    return loop(struct).matchall((field, type) => {
        // Ignore empty fields unless strict
        if (typeof json[field] === 'undefined') {
            return ! strict;
        }

        // type can be an array of subtype (array, object) and
        // its internal struct.
        if (Array.isArray(type)) {
            // Make sure it's an array or object
            if (! validateStruct(json[field], type[0], strict)) {
                return false;
            }

            // Make sure all its items match
            return loop(json[field]).matchall((item) => {
                validateStruct(item, type[1], strict);
            });
        }

        // Get the data and validate it
        return validateStruct(json[field], type, strict);
    });
}
/*export*/ validateStruct;
