# EasyHome
Home automation using NodeMCU.
Control upto 5 devices.

![](https://github.com/shajanjp/shajanjp.github.io/raw/master/uploads/easyhome-homepage.jpg)

## PIN Connections

GPIO 14 => Device 1
GPIO 12 => Device 2
GPIO 13 => Device 3
GPIO 15 => Device 4
GPIO 3 => Device 5

## API

### Get device details

#### Request :
`GET /devices`

#### Response :
```json
[
    {
        "device": 1,
        "status": 0
    },
    {
        "device": 2,
        "status": 1
    },
    {
        "device": 3,
        "status": 1
    },
    {
        "device": 4,
        "status": 0
    },
    {
        "device": 5,
        "status": 0
    }
]
```

### Turn device 1 on

#### Request :
`GET /devices?device=1&status=1`
#### Response :
```json
{
    "device": 1,
    "status": 1
}
```

### Turn off all devices

#### Request :
`GET /devices?device=all&state=0`
#### Response :
```json
[
    {
        "device": 1,
        "status": 0
    },
    {
        "device": 2,
        "status": 0
    },
    {
        "device": 3,
        "status": 0
    },
    {
        "device": 4,
        "status": 0
    },
    {
        "device": 5,
        "status": 0
    }
]
```