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
`GET /plugs`

#### Response :
```json
[
    {
        "plug": 1,
        "status": 0
    },
    {
        "plug": 2,
        "status": 1
    },
    {
        "plug": 3,
        "status": 1
    },
    {
        "plug": 4,
        "status": 0
    },
    {
        "plug": 5,
        "status": 0
    }
]
```

### Turn device 1 on

#### Request :
`GET /plugs?plug=1&status=1`
#### Response :
```json
{
    "plug": 1,
    "status": 1
}
```

### Turn device 2 off

#### Request :
`GET /plugs?plug=2&status=0`
#### Response :
```json
{
    "plug": 2,
    "status": 0
}
```
