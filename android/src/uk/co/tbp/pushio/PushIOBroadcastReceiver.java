package uk.co.tbp.pushio;

import android.app.Activity;
import android.content.*;
public class PushIOBroadcastReceiver extends BroadcastReceiver
{

    public PushIOBroadcastReceiver()
    {
    }

    public void onReceive(Context context, Intent intent)
    {
        PushIOGCMIntentService.runIntentInService(context, intent);
        setResult(-1, null, null);
    }
}
