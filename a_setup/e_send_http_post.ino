void sendPost (String hud, String temp, String light_status)
{   
  HTTPClient http; // make http object
  http.begin(client, SERVER_PATH); // connect to the server ip address
  http.addHeader("Content-Type", "application/json");
  String post_data = "{\"hud\" : \"" + hud + "\", \"temp\" : \"" + temp + "\", \"location\" : \"" + LOCATION + "\", \"light\" : \"" + light_status + "\"}" ;
  int httpResponseCode = http.POST(post_data);
  Serial.println (post_data);
  Serial.println ("send post, response: " + (String)httpResponseCode);
  http.end();
}
