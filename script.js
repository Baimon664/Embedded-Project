var status=0; // 1 is on
var mic=0;  // 0 is on

$(document).ready(function(e) {
    client = new Paho.MQTT.Client("mqtt.netpie.io",443, "");
    var options = {
        useSSL: true,
        userName: "",
        password: "",
        onSuccess:onConnect,
        onFailure:doFail
    }
    client.connect(options);

    function onConnect() {
        $("#status").text("CONNECTED").removeClass().addClass("connected");
        client.subscribe("@msg/status");
    }

    function doFail(e){
        console.log(e);
    }

    client.onMessageArrived = function(message) {
        console.log("Message Arrived: " + message.payloadString);
        if (message.payloadString == "one"){
            status=1;
            mic=0;
            $("#led").text("ON").removeClass().addClass("on");
            $("#microphone").text("ENABLE").removeClass().addClass("enable");
        }
        else if(message.payloadString == "offe"){
            status=0;
            mic=0;
            $("#led").text("OFF").removeClass().addClass("off");
            $("#microphone").text("ENABLE").removeClass().addClass("enable");
        }
        else if(message.payloadString == "ond"){
            status=1;
            mic=1;
            $("#led").text("ON").removeClass().addClass("on");
            $("#microphone").text("DISABLE").removeClass().addClass("disable");
        }
        else if(message.payloadString == "offd"){
            status=0;
            mic=1;
            $("#led").text("OFF").removeClass().addClass("off");
            $("#microphone").text("DISABLE").removeClass().addClass("disable");
        }
    }

    $("#toggle-id").click(function(e) {
        console.log("click");
        if(status == 1){
            mqttSend("@msg/led", "LEDOFF");
            status=0;
        }
        else{
            mqttSend("@msg/led", "LEDON");
            status=1;
        }
    });

    $("#mic-id").click(function(e) {
        console.log("click");
        if(mic == 0){
            mqttSend("@msg/led", "MICOFF");
            mic=1;
        }
        else{
            mqttSend("@msg/led", "MICON");
            mic=0;
        }
    });
    
});
var mqttSend = function(topic, msg) {
    var message = new Paho.MQTT.Message(msg);
    message.destinationName = topic;
    client.send(message);
}
