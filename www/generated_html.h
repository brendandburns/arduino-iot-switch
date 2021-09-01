#ifndef __DATA_H__
#define __DATA_H__
const char *index_html = R"INDEX_HTML(
<html>

<head>
    <link rel="stylesheet" href="https://fonts.googleapis.com/icon?family=Material+Icons">
    <link rel="stylesheet" href="https://code.getmdl.io/1.3.0/material.indigo-pink.min.css">
    <script src="https://code.getmdl.io/1.3.0/material.min.js"></script>
    <script src="https://ajax.aspnetcdn.com/ajax/jQuery/jquery-3.6.0.min.js"></script>
    <script lang="javascript" src="./script.js">
    </script>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>

<body>
    <div class="mdl-layout mdl-js-layout mdl-layout--fixed-header">
        <header class="mdl-layout__header">
            <div class="mdl-layout__header-row">
                <span class="mdl-layout-title">IoT Shed</span>
            </div>
        </header>
        <main class="mdl-layout__content" style="padding: 10px 30px">
            <div class="mdl-card mdl-shadow--2dp">
                <div class="mdl-card__title">
                    <h2 class="mdl-card__title-text">Switches</h2>
                </div>
                <div class="mdl-card__supporting-text" id='switches'>
                </div>
            </div>
        </main>
    </div>
    <script lang="javascript">
        // TODO: Figure out a better way to test that the material design objects are loaded.
        // setTimeout(loadStatus, 100);
        createSwitches();
    </script>
</body>

</html>
)INDEX_HTML";
const char *script_js = R"SCRIPT_JS(
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
)SCRIPT_JS";
#endif // __DATA_H__
