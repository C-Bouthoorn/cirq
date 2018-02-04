/* globals require */

require.config({
    baseUrl: 'assets/js',

    shim: {
        utils: {
            deps: [ ]
        },

        Template: {
            deps: [ 'utils' ],
            exports: 'Template',
        },

        Circuit: {
            deps: [ 'utils' ],
            exports: 'Circuit',
        },

        Wire: {
            deps: [ 'utils' ],
            exports: 'Wire',
        },

        Port: {
            deps: [ 'Wire' ],
            exports: 'Port',
        },

        CustomGate: {
            deps: [ 'utils', 'Template' ],
            exports: 'CustomGate',
        },

        Gate: {
            deps: [ 'Port', 'utils' ],
            exports: 'Gate',
        },

        wireEvents: {
            deps: [ 'Wire' ],
        },

        dragAndDropEvents: {
            deps: [ 'Gate', 'Wire', 'Port', 'utils' ],
        },

        indexx: {
            deps: [ 'dragAndDropEvents', 'wireEvents', 'Circuit', 'CustomGate', 'utils', 'Wire', 'Gate' ],
        },
    },
});
