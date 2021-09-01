function handleClick(which) {
    const id = 'switch-' + which;
    const elt = document.getElementById(id);
    var verb = elt.checked ? 'on' : 'off';

    $.ajax(`/switch/${which}/${verb}`);
}

function updateStatus(data) {
    data.forEach((data) => {
        const elt = document.getElementById(`switch-${data.ix}`);
        if (!elt) {
            return;
        }
        if (data.on) {
            elt.parentElement.MaterialSwitch.on();
        } else {
            elt.parentElement.MaterialSwitch.off();
        }
    });
}

function loadStatus() {
    $.ajax('/switch', { 'dataType': 'json' }).done(updateStatus);
}

function makeSwitches(data) {
    data.forEach(makeSwitch);
}

function makeSwitch(data) {
/*
    <label class="mdl-switch mdl-js-switch mdl-js-ripple-effect" for="switch-1">
        <span class="mdl-switch__label">Fountain</span>
        <input type="checkbox" id="switch-1" class="mdl-switch__input" checked onclick="handleClick('1')">
    </label>
*/
    var label = document.createElement('label');
    label.className = 'mdl-switch mdl-js-switch mdl-js-ripple-effect';
    label.for = 'switch-' + data.ix;

    var span = document.createElement('span');
    span.className = 'mdl-switch__label';
    span.appendChild(document.createTextNode(data.name));

    var input = document.createElement('input');
    input.type = 'checkbox';
    input.id = 'switch-' + data.ix;
    input.className = 'mdl-switch__input';
    input.checked = data.on;
    input.onclick = () => handleClick(data.ix);

    label.appendChild(span);
    label.appendChild(input);

    componentHandler.upgradeElement(label);
    var container = document.getElementById('switches');
    container.appendChild(label);
    container.appendChild(document.createElement('p'));
}

function createSwitches() {
    $.ajax('/switch', { 'dataType': 'json' }).done(makeSwitches);
}