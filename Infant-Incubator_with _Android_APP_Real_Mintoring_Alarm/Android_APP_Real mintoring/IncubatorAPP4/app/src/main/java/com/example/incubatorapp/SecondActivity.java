package com.example.incubatorapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.io.IOException;
import java.io.InputStream;
import java.util.Set;
import java.util.UUID;

public class SecondActivity extends AppCompatActivity {

    TextView temperatureValue, humidityValue, statusMessage;
    BluetoothAdapter bluetoothAdapter;
    BluetoothSocket btSocket;
    InputStream inputStream;
    Handler handler = new Handler();

    final String HC05_ADDRESS = "00:22:04:00:C1:00"; // Replace with your HC-05 MAC address
    final UUID BT_MODULE_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.second);

        temperatureValue = findViewById(R.id.temperatureValue);
        humidityValue = findViewById(R.id.humidityValue);
        statusMessage = findViewById(R.id.statusMessage);

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        connectToBluetoothDevice();
    }

    private void connectToBluetoothDevice() {
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            // TODO: Consider calling
            //    ActivityCompat#requestPermissions
            // here to request the missing permissions, and then overriding
            //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
            //                                          int[] grantResults)
            // to handle the case where the user grants the pe
            // rmission. See the documentation
            // for ActivityCompat#requestPermissions for more details.
            return;
        }
        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();

        for (BluetoothDevice device : pairedDevices) {
            if (device.getAddress().equals(HC05_ADDRESS)) {
                try {
                    if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                        // TODO: Consider calling
                        //    ActivityCompat#requestPermissions
                        // here to request the missing permissions, and then overriding
                        //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
                        //                                          int[] grantResults)
                        // to handle the case where the user grants the permission. See the documentation
                        // for ActivityCompat#requestPermissions for more details.
                        return;
                    }
                    btSocket = device.createRfcommSocketToServiceRecord(BT_MODULE_UUID);
                    btSocket.connect();
                    statusMessage.setText("Connected to HC-05");
                    inputStream = btSocket.getInputStream();
                    listenForData();
                } catch (IOException e) {
                    statusMessage.setText("Connection Failed");
                    e.printStackTrace();
                }
                break;
            }
        }
    }

    private void listenForData() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                byte[] buffer = new byte[256];
                int bytes;
                StringBuilder dataBuilder = new StringBuilder();

                while (true) {
                    try {
                        if ((bytes = inputStream.read(buffer)) > 0) {
                            String incomingData = new String(buffer, 0, bytes);
                            dataBuilder.append(incomingData);

                            // Parse data when a delimiter is found
                            if (dataBuilder.toString().contains(";")) {
                                String fullData = dataBuilder.toString();
                                dataBuilder.setLength(0);

                                // Example: T:25.5;H:60.3;
                                String[] parts = fullData.split(";");
                                String temp = "--", hum = "--";

                                for (String part : parts) {
                                    if (part.startsWith("T:")) {
                                        temp = part.substring(2);
                                    } else if (part.startsWith("H:")) {
                                        hum = part.substring(2);
                                    }
                                }

                                String finalTemp = temp;
                                String finalHum = hum;
                                handler.post(() -> {
                                    temperatureValue.setText(finalTemp + " °C");
                                    humidityValue.setText(finalHum + " %");

                                    // Notify if abnormal
                                    double tempValue = Double.parseDouble(finalTemp);
                                    double humValue = Double.parseDouble(finalHum);

                                    if (tempValue > 38 || tempValue < 35 || humValue > 80 || humValue < 30) {
                                        statusMessage.setText("Warning: Abnormal Condition!");
                                    } else {
                                        statusMessage.setText("Status: Normal");
                                    }
                                });
                            }
                        }
                    } catch (IOException e) {
                        break;
                    }
                }
            }
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        try {
            if (btSocket != null) btSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
