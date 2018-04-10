let offStateColor = 'ff4444';
let onStateColor = '#22ff77';
let loadingStateColor = '#ffdd66';

let makeSwitch = function(device, status) {
  if (status == 0) {
$(device).find('.image svg g path').css({fill: offStateColor});
} else if (status == 1) {
$(device).find('.image svg g path').css({fill: onStateColor});
} else {
$(device).find('.image svg g path').css({fill: loadingStateColor});
}
};

let nextState = function(currentState) {
  if (currentState == 1) {
return 0;
} else {
return 1;
}
};

let checkDeviceStatus = function() {
  $.get('http://' + $('#mcuip').val() + '/devices', function(data) {
    if (data) {
      data.forEach(function(d) {
        makeSwitch(d.device, d.status);
      });
    }
  });
};

$('.device').on('click', function() {
  let currentDevice = $(this);
  let data = {};
  data.device = currentDevice.data('device');
  data.status = nextState(currentDevice.data('state'));
  makeSwitch(this, 1);
  $.get('http://' + $('#mcuip').val() + '/devices', data, function(response) {
    makeSwitch(response.device, response.state);
  });
});

$('.color').on('click', function() {
  let currentColor = $(this);
  let effect = $('input[name=effect]:checked').val();
  let colorRGB = {};
  colorRGB.r = currentColor.data('r');
  colorRGB.g = currentColor.data('g');
  colorRGB.b = currentColor.data('b');
  colorRGB.effect = effect;
  $.get('http://' + $('#mcuip').val() + '/strip', colorRGB, function(response) {
    console.log('Color changed !');
  });
});

$('#check-device-status').on('click', function() {
  checkDeviceStatus();
});

$('#connect-device').on('click', function() {
  checkDeviceStatus();
});

$('#turn-off-devices').on('click', function() {
  $.get('http://' + $('#mcuip').val() + '/devices?device=all&status=0', function(data) {
    data.forEach(function(d) {
      makeSwitch(d.device, d.status);
    });
  });
});
