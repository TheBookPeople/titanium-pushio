---
layout: docs
title: Installation
prev_section: download
permalink: /docs/install/
---


### Install - Mac OS X

Extract distribution zip file

Copy modules/iphone/uk.co.tbp.pushio to 

~/Library/Application Support/Titanium/modules/iphone

### Register the Module

Register the module with your application by editing `tiapp.xml` and adding your module.
Example:

{% highlight xml %}
<modules>
 <module version="1.0">uk.co.tbp.pushio</module>
</modules>
{% endhighlight %}

## Accessing the push-io Module

To access this module from JavaScript, you would do the following:

{% highlight javascript %}
var push_io = require("uk.co.tbp.pushio");
{% endhighlight %}

The push_io variable is a reference to the Module object.

When you run your project, the compiler will combine your module along with its dependencies and assets into the application.

### pushio_config_debug.json

The  pushio_config_debug.json file that is created in the PushIO management interface needs to be copied to
the assets folder of your titanium application.
