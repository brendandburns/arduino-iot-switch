function handleClick(which) {
    const id = 'switch-' + which;
    const elt = document.getElementById(id);
    var verb = elt.checked ? 'on' : 'off';

    $.ajax(`/switch/${which}/${verb}`);
}

function updateStatus(data) {
    data.forEach((data) => {
        const elt = document.getElementById(`switch-${data.ix + 1}`);
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