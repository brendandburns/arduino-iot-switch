function handleClick(which) {
    const id = 'switch-' + which;
    const elt = document.getElementById(id);
    var verb = elt.checked ? 'on' : 'off';
    
    console.log(`turning ${verb} ${which}`);

    $.ajax(`/switch/${which}/${verb}`);
}