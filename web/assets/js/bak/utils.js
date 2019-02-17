// Some values which should reflect CSS
window.GATE_WIDTH = 75;
window.WIRE_WIDTH = 4;
window.GATE_BORDER = 3;


// Based on https://stackoverflow.com/a/105074/4141219
function guid() {
    function s4() {
        return Math.floor((Math.random() + 1) * 0x10000)
        .toString(16)
        .substring(1);
    }

    return s4() + s4();
}


function calculateLength(ax, ay, bx, by) {
    return Math.sqrt(Math.pow(bx - ax, 2) + Math.pow(by - ay, 2));
}


function calculateRadians(ax, ay, bx, by) {
    return Math.atan((by - ay) / (bx - ax));
}


function xRound(x, decimals) {
    let fac = Math.pow(10, decimals);
    return Math.round(x * fac) / fac;
}


function HTMLToElem(html) {
    let tmp = document.createElement('div');
    tmp.innerHTML = html;
    return tmp.children[0];
}


function LOOP_OBJ(obj) {
    return {
        forEach: function(callback) {
            for(let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                callback(k, v);
            }
        },


        map: function(callback) {
            let results = {};

            for(let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                results[k] = callback(k, v);
            }

            return LOOP_OBJ(results);
        },


        filter: function(callback) {
            let results = {};

            for(let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                let v = obj[k];

                if (callback(k, v)) {
                    results[k] = v;
                }
            }

            return LOOP_OBJ(results);
        },


        keys: function() {
            let keys = [];

            LOOP_OBJ(obj).forEach((key) => {
                keys.push(key);
            })

            return keys;
        },


        values: function() {
            let values = [];

            LOOP_OBJ(obj).forEach((_, value) => {
                values.push(value);
            })

            return values;
        },


        removeLoop: function() {
            return obj;
        },


        length: function() {
            let len = 0;

            for(let k in obj) {
                if (! obj.hasOwnProperty(k)) continue;
                len++;
            }

            return len;
        }
    };
}


// based on https://stackoverflow.com/a/40292774/4141219
function CLONE(o) {
    let references = [];
    let cachedResults = [];

    function clone(obj) {
        if (typeof obj !== 'object') return obj;

        let index = references.indexOf(obj);
        if (index !== -1) return cachedResults[index];
        references.push(obj);

        // obj.constructor ? new obj.constructor() :
        let result = Array.isArray(obj) ? [] : {};
        cachedResults.push(result);

        for (var key in obj) {
            if (obj.hasOwnProperty(key)) {
                result[key] = clone(obj[key]);
            }
        }

        return result;
    }

    return clone(o);
}
