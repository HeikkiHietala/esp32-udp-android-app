package com.example.ESP32CAR

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import com.example.ESP32CAR.R.id.txtShowIP
import kotlinx.android.synthetic.main.activity_settings.*
import org.w3c.dom.Text


class Settings : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        var editEnterip =  mySettings.RemoteHost
        var editEnterport= mySettings.RemotePort
        setContentView(R.layout.activity_settings)
        this.editEnterip.setText(mySettings.RemoteHost.toString())
        this.editEnterport.setText(mySettings.RemotePort.toString())

        btnSetip.setOnClickListener {
            var finalIP =findViewById<EditText>(R.id.editEnterip)
            var textFromEditText = finalIP.text.toString() // access text this way
            mySettings.RemoteHost = textFromEditText.toString()
            var finalPort =findViewById<EditText>(R.id.editEnterport)
            var textfromEnterport = finalPort.text.toString().toInt()
            //finalPort = "1238"
            mySettings.RemotePort = textfromEnterport.toInt()

            val intent = Intent(this, MainActivity::class.java)
            startActivity(intent)
        }
        btnCancelip.setOnClickListener {
            var editEnterip =  "null"
            var editEnterport = "null"
            myTargetIP = editEnterip
            myTargetPort = editEnterport
            val intent = Intent(this, MainActivity::class.java);
            intent.putExtra("myTargetIP", myTargetIP)
            intent.putExtra("myTargetPort", myTargetPort)
            startActivity(intent)
        }
    }
}
