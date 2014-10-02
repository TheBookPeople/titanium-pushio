// open a single window
// var win = Ti.UI.createWindow({
//   backgroundColor:'white'
// });
//
// var label = Ti.UI.createLabel();
// win.add(label);
// //win.open();
//
//
//
// var pushio;
//
// if (OS_IOS) {
//   pushio = require('uk.co.tbp.pushio');
//   pushio.registerForPushNotifications({
//     // Specifies which notifications to receive
//     types : [Ti.Network.NOTIFICATION_TYPE_BADGE,
//              Ti.Network.NOTIFICATION_TYPE_ALERT,
//              Ti.Network.NOTIFICATION_TYPE_SOUND],
//
//     success : function(e) {
//       alert('Divice token : ' + JSON.stringify(e));
//     },
//
//     error : function(e) {
//       alert('error: ' + JSON.stringify(e));
//     },
//
//     callback : function(e) {
//       callTests();
//     }
//   });
//
// }else{
//   assert(false,"Only iOS supported!")
// }
// }

// function callTests(){
//   pushio.unregisterAllCategories();
//   testRegisterCategory();
// }
//
// function testRegisterCategory(){
//   pushio.registerCategory('Bannana');
//   var cat =  pushio.allRegisteredCategories();
//
//   assert(cat.length==1,"should only have one category");
//   assert(cat[0]==='Bannana',"Bannana should have been registered");
// }

// function testRegisterCategories(){
//   pushio.registerCategory('Bannana');
//   assert(isRegisteredForCategory('Bannana'),"Bannana should have been registered")
// }
//
//
//
//   pushio.registerCategories(['Socks','Burgers']);
//
//   pushio.unregisterCategory('Bannana');
//   pushio.unregisterCategories(['Socks','Burgers']);
//
//   pushio.unregisterCategory('Bannana');
//   pushio.unregisterCategories(['Socks','Burgers']);
//
//   pushio.unregisterAllCategories();
//
//   pushio.isRegisteredForCategory();
//
//   pushio.allRegisteredCategories();
//
//   pushio.registerUserID();
//
//   pushio.unregisterUserID();
//
//   pushio.registeredUserID();
//
//   pushio.isRegisteredForUserID();
//
//   pushio.trackEngagementCustomMetric();
//
//   pushio.registerUserID('a@bb.com');
//
// }

function assert(condition, message) {
    if (!condition) {
        message = message || "Assertion failed";
        if (typeof Error !== "undefined") {
            throw new Error(message);
        }
        throw message; // Fallback
    }
}
