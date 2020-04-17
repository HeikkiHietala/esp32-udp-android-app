package com.example.ESP32CAR

import android.content.Intent
import android.os.Bundle
import android.os.StrictMode
import android.view.View
import android.widget.EditText
import android.widget.TextView
import java.io.IOException
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

var mySpeed: Int = 0
var myDirection:Int = 0
var myTargetIP = "not set"
var myTargetPort = "not set"
var myUDP:String = ""

public class SoftOptions {
    var RemoteHost: String = "192.168.10.61"
    var RemotePort: Int = 1234

    constructor()
    init{}
}

// Global
val mySettings = SoftOptions()

class MainActivity : AppCompatActivity() {

    fun sendUDP(messageStr: String) {
        // Hack Prevent crash (sending should be done using an async task)
        val policy = StrictMode.ThreadPolicy.Builder().permitAll().build()
        StrictMode.setThreadPolicy(policy)
        try {
            //Open a port to send the package
            val socket = DatagramSocket()
            socket.broadcast = true
            val sendData = messageStr.toByteArray()
            val sendPacket = DatagramPacket(sendData, sendData.size, InetAddress.getByName(mySettings.RemoteHost), mySettings.RemotePort)
            socket.send(sendPacket)
        } catch (e: IOException) {
            //            Log.e(FragmentActivity.TAG, "IOException: " + e.message)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var intent = intent
        val IPtoshow = intent.getStringExtra("myTargetIP")
        val resultTv = this.findViewById<TextView>(R.id.txtShowIP)
        resultTv.text = "Target device IP is set as " + mySettings.RemoteHost
        val PortToShow = intent.getStringExtra("myTargetPort")
        val resultTvPort = this.findViewById<TextView>(R.id.txtShowPort)
        resultTvPort.text = "Target device port is set as " + mySettings.RemotePort

        btnIncrease.setOnClickListener {
            mySpeed += 1
            if (mySpeed > 10) mySpeed = 10
            val SpeedtoShowTv = this.findViewById<TextView>(R.id.txtSpeed)
            SpeedtoShowTv.text = mySpeed.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }
        btnDecrease.setOnClickListener {
            mySpeed -= 1
            if (mySpeed < -10) mySpeed = -10
            val SpeedtoShowTv = this.findViewById<TextView>(R.id.txtSpeed)
            SpeedtoShowTv.text = mySpeed.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }

        btnStop.setOnClickListener {
            mySpeed = 0
            myDirection = 0
            val SpeedtoShowTv = this.findViewById<TextView>(R.id.txtSpeed)
            SpeedtoShowTv.text = mySpeed.toString()
            val DirtoShowTv = this.findViewById<TextView>(R.id.txtDirection)
            DirtoShowTv.text = myDirection.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }
        btnLeft.setOnClickListener {
            myDirection -= 1
            if (myDirection < -5) myDirection = -5
            val DirtoShowTv = this.findViewById<TextView>(R.id.txtDirection)
            DirtoShowTv.text = myDirection.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }

        btnCenter.setOnClickListener {
            myDirection = 0
            val DirtoShowTv = this.findViewById<TextView>(R.id.txtDirection)
            DirtoShowTv.text = myDirection.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }
        btnRight.setOnClickListener {
            myDirection += 1
            if (myDirection > 5) myDirection = 5
            val DirtoShowTv = this.findViewById<TextView>(R.id.txtDirection)
            DirtoShowTv.text = myDirection.toString()
            myUDP = mySpeed.toString() + "," + myDirection.toString()
            sendUDP(myUDP)
        }
        btnSettings.setOnClickListener {
            val intent = Intent(this, Settings::class.java);
            startActivity(intent)
        }
    }
}



