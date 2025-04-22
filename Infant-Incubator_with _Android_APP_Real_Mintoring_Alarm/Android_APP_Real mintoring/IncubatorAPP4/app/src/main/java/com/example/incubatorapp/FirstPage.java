
package com.example.incubatorapp;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import androidx.appcompat.app.AppCompatActivity;
public class FirstPage extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.firstpage);
        // Your splash or intro layout

            // Wait 2 seconds, then open FirstPage
            new Handler().postDelayed(() -> {
                Intent intent = new Intent(FirstPage.this,MainActivity.class);
                startActivity(intent);
                finish();  // Close the splash page
            }, 1000);
    }
}

