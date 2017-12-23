module.exports = {
    env: {
        browser: true,
        es6: true,
    },

    extends: 'eslint:recommended',

    rules: {
        'no-console': 'off',
        'no-debugger': 'off',

        'no-unused-vars': 'warn',
        'no-undef': 'warn',
        'no-extra-semi': 'warn',
        'no-unexpected-multiline': 'warn',
        'no-empty': 'warn',
    }
};
