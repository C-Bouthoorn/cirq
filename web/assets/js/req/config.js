/* globals require */

require.config({
    baseUrl: 'assets/js',

    shim: {
        utils: {
            deps: [ ]
        },

        Circuit: {
            deps: [ 'utils', 'Component', 'Gate', 'Wire', 'Port' ],
            exports: 'Circuit',
        },

        Gate: {
            deps: [ 'utils', 'Component', 'Port' ],
            exports: 'Gate',
        },

        Wire: {
            deps: [ 'utils', 'Component', 'Port' ],
            exports: 'Wire',
        },

        Port: {
            deps: [ 'utils', 'Component' ],
            exports: 'Port',
        },

        indexx: {
            deps: [ 'utils', 'eventDragAndDrop', 'Circuit' ],
        },
    },
});
