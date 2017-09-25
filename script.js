var toggleBlink = function(device){
	device.find('.toggle').removeClass('grey').addClass('yellow');
	setTimeout(function(){
		device.find('.toggle').removeClass('yellow').addClass('grey');
	}, 500);
	setTimeout(function(){
		device.find('.toggle').removeClass('grey').addClass('yellow');
	}, 1000);
	setTimeout(function(){
		device.find('.toggle').removeClass('yellow').addClass('grey');
	}, 1500);
	setTimeout(function(){
		device.find('.toggle').removeClass('grey').addClass('yellow');
	}, 2000);
	setTimeout(function(){
		device.find('.toggle').removeClass('yellow').addClass('grey');
	}, 2500);
}

var makeSwitch = function(sno, state){
	var device = $('#device-' + sno);
	if(state == "0"){
		device.find('.switch').removeClass('green').addClass('red');
		device.find('.toggle').removeClass('yellow').addClass('grey');
		device.data('state', "0");
	}
	else if(state == "1"){
		device.find('.switch').removeClass('red').addClass('green');
		device.data('state', "1");
	}
	else if(state == "2"){
		toggleBlink(device);
	}
}

var nextState = function(current_state){
	if (current_state == 1) 
		return 0;
	else
		return 1;
}

var checkDeviceStatus = function(){
	$.get("http://" + $('#mcuip').val() + "/devices", function(data) {
		data.forEach(function(d){
			makeSwitch(d.device, d.status);
		});
	});
}

$('.switch').on('click', function(){
	let current_device = $(this).closest('.device');
	let data = {};
	data.device = current_device.data('device');
	data.status = nextState(current_device.data('state'));
	$.get("http://" + $('#mcuip').val() + "/devices", data, function(response){
		makeSwitch(response.device, response.state);
	});
});

$('.toggle').on('click', function(){
	let current_device = $(this).closest('.device');
	let data = {};
	data.device = current_device.data('device');
	data.status = 2;
	makeSwitch(data.device, data.status);
	$.get("http://" + $('#mcuip').val() + "/devices", data, function(response){
		makeSwitch(response.device, response.state);
	});
});

$('#check-device-status').on('click', function(){
	checkDeviceStatus();
});

$('#connect-device').on('click', function(){
	$.get("http://" + $('#mcuip').val() + "/devices", function(data) {
		if(data){
			data.forEach(function(d){
				makeSwitch(d.device, d.status);
			});
			$('#connect-device').removeClass('primary').addClass('green').html('<i class="refresh icon"></i>Connected');
		}
	});
});

$('#turn-off-devices').on('click', function(){
	$.get("http://" + $('#mcuip').val() + "/devices?device=all&status=0", function(data) {
		data.forEach(function(d){
			makeSwitch(d.device, d.status);
		});
	});
});