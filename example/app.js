// open a single window
var win = Ti.UI.createWindow({
	backgroundColor:'white'
});

var label = Ti.UI.createLabel();
win.add(label);
win.open();

var push_io = require('uk.co.tbp.pushio');
Ti.API.info("module is => " + push_io);
