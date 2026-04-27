package com.zombie.shooter

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import org.godotengine.godot.Godot
import org.godotengine.godot.plugin.GodotPlugin
import org.godotengine.godot.plugin.SignalInfo

/**
 * Kotlin Godot Plugin for Zombie Shooter
 * Provides Android-native functionality
 */
class ZombiePlugin(godot: Godot) : GodotPlugin(godot) {
    
    companion object {
        private const val TAG = "ZombiePlugin"
    }
    
    private var currentActivity: Activity? = null
    
    override fun getPluginName(): String {
        return "ZombiePlugin"
    }
    
    override fun getPluginMethods(): List<String> {
        return listOf(
            "getDeviceInfo",
            "showToast",
            "shareScore",
            "saveHighScore",
            "getHighScore",
            "vibrate",
            "vibratePattern"
        )
    }
    
    override fun getPluginSignals(): Set<SignalInfo> {
        val signals = mutableSetOf<SignalInfo>()
        signals.add(SignalInfo("score_shared", String::class.java))
        signals.add(SignalInfo("high_score_saved", Integer::class.java))
        signals.add(SignalInfo("vibration_complete"))
        return signals
    }
    
    override fun onMainCreate(activity: Activity?): View? {
        currentActivity = activity
        return null
    }
    
    /**
     * Get Android device information
     */
    fun getDeviceInfo(): Dictionary {
        val info = Dictionary()
        info["manufacturer"] = android.os.Build.MANUFACTURER
        info["model"] = android.os.Build.MODEL
        info["android_version"] = android.os.Build.VERSION.RELEASE
        info["sdk_version"] = android.os.Build.VERSION.SDK_INT
        info["device_name"] = android.os.Build.DEVICE
        info["package_name"] = currentActivity?.packageName ?: "unknown"
        return info
    }
    
    /**
     * Show Android Toast message
     */
    fun showToast(message: String, duration: Int = 0) {
        currentActivity?.runOnUiThread {
            android.widget.Toast.makeText(
                currentActivity,
                message,
                if (duration == 1) android.widget.Toast.LENGTH_LONG 
                else android.widget.Toast.LENGTH_SHORT
            ).show()
        }
    }
    
    /**
     * Share score via Android Share Intent
     */
    fun shareScore(score: Int, message: String) {
        currentActivity?.runOnUiThread {
            val shareIntent = Intent(Intent.ACTION_SEND).apply {
                type = "text/plain"
                putExtra(Intent.EXTRA_SUBJECT, "Zombie Shooter Score!")
                putExtra(Intent.EXTRA_TEXT, "$message\nScore: $score")
            }
            
            val chooser = Intent.createChooser(shareIntent, "Share Score")
            currentActivity?.startActivity(chooser)
            
            emitSignal("score_shared", message)
        }
    }
    
    /**
     * Save high score to SharedPreferences
     */
    fun saveHighScore(score: Int): Boolean {
        currentActivity?.let { activity ->
            val prefs = activity.getSharedPreferences("ZombieShooter", android.content.Context.MODE_PRIVATE)
            val editor = prefs.edit()
            val currentHigh = prefs.getInt("high_score", 0)
            
            if (score > currentHigh) {
                editor.putInt("high_score", score)
                editor.apply()
                emitSignal("high_score_saved", score)
                return true
            }
        }
        return false
    }
    
    /**
     * Get saved high score
     */
    fun getHighScore(): Int {
        currentActivity?.let { activity ->
            val prefs = activity.getSharedPreferences("ZombieShooter", android.content.Context.MODE_PRIVATE)
            return prefs.getInt("high_score", 0)
        }
        return 0
    }
    
    /**
     * Vibrate device
     */
    fun vibrate(milliseconds: Long) {
        currentActivity?.let { activity ->
            val vibrator = activity.getSystemService(android.content.Context.VIBRATOR_SERVICE) 
                as android.os.Vibrator
            
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                vibrator.vibrate(
                    android.os.VibrationEffect.createOneShot(
                        milliseconds,
                        android.os.VibrationEffect.DEFAULT_AMPLITUDE
                    )
                )
            } else {
                @Suppress("DEPRECATION")
                vibrator.vibrate(milliseconds)
            }
        }
    }
    
    /**
     * Vibrate with pattern
     */
    fun vibratePattern(pattern: LongArray, repeat: Int) {
        currentActivity?.let { activity ->
            val vibrator = activity.getSystemService(android.content.Context.VIBRATOR_SERVICE)
                as android.os.Vibrator
            
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                vibrator.vibrate(
                    android.os.VibrationEffect.createWaveform(pattern, repeat)
                )
            } else {
                @Suppress("DEPRECATION")
                vibrator.vibrate(pattern, repeat)
            }
            emitSignal("vibration_complete")
        }
    }
}

// Extension function for Dictionary
fun Dictionary() = org.godotengine.godot.Dictionary()
