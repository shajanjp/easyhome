# EasyHome
Home automation using NodeMCU.
Control upto 5 devices.

![](https://i.imgur.com/E4ydN86.png)

## PIN Connections
// Switches d1, d2, d5, d6 || 5, 4, 14, 12
Device 1 => GPIO 5 =>  D1
Device 2 => GPIO 4 =>  D2
Device 3 => GPIO 14 =>  D5
Device 4 => GPIO 12 =>  D6

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
