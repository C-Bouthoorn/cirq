/* global LOOP_OBJ, define */

window.GATE_INNER_TEMPLATE = new Template("{{TYPE}}<br>&nbsp; &lt; {{IN}}<br>&nbsp; &gt; {{OUT}}");


function Template(html) {
    this.html = html;
}


Template.prototype.apply = function(values) {
    let html = this.html;

    // Apply all values
    LOOP_OBJ(values).forEach((k, v) => {
        html = html.replace(new RegExp('{{\\s*' + k + '\\s*}}', 'gi'), v);
    });

    return html;
};


define(() => Template);
