let offStateColor = "ff4444";
let onStateColor = "#22ff77";
let loadingStateColor = "#ffdd66";

var makeSwitch = function(device, status){
	if(status == 0)
		$(device).find('.image svg g path').css({ fill: offStateColor });
	else if(status == 1)
		$(device).find('.image svg g path').css({ fill: onStateColor });
	else
		$(device).find('.image svg g path').css({ fill: loadingStateColor });
}

var nextState = function(currentState){
	if (currentState == 1) 
		return 0;
	else
		return 1;
}

var checkDeviceStatus = function(){
	$.get("http://" + $('#mcuip').val() + "/plugs", function(data) {
		if(data){
			data.forEach(function(d){
				makeSwitch(d.device, d.status);
			});
		}
	});
}

$('.device').on('click', function(){
	let current_device = $(this);
	let data = {};
	data.plug = current_device.data('device');
	data.status = nextState(current_device.data('state'));
	makeSwitch(this, 1);
	$.get("http://" + $('#mcuip').val() + "/devices", data, function(response){
		makeSwitch(response.device, response.state);
	});
});

$('.color').on('click', function(){
	let currentColor = $(this);
	let colorRGB = {};
	colorRGB.r = currentColor.data('r');
	colorRGB.g = currentColor.data('g');
	colorRGB.b = currentColor.data('b');
	colorRGB.type = "snake";
	$.get("http://" + $('#mcuip').val() + "/strip", colorRGB, function(response){
		console.log("Color changed !");
	});
});

$('#check-device-status').on('click', function(){
	checkDeviceStatus();
});

$('#connect-device').on('click', function(){
	checkDeviceStatus();
});

$('#turn-off-devices').on('click', function(){
	$.get("http://" + $('#mcuip').val() + "/plugs?device=all&status=0", function(data) {
		data.forEach(function(d){
			makeSwitch(d.device, d.status);
		});
	});
});