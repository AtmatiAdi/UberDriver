using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class CameraController : MonoBehaviour
{

    public WebCamTexture camera;
    public GameObject plane;
    public Text text;
    public InputField framerate;
    public InputField height;
    public InputField width;
    public Button set;
    int amount;

    // Start is called before the first frame update
    IEnumerator Start()
    {
        yield return Application.RequestUserAuthorization(UserAuthorization.WebCam);
        camera = new WebCamTexture();
        plane.GetComponent<Renderer>().material.mainTexture = camera;
        //Camera.main.targetTexture = camera;
        camera.requestedFPS = 3;
        camera.Play();
        WebCamDevice[] devices = WebCamTexture.devices;
        for (int i = 0; i < devices.Length; i++)
            text.text += devices[i].name;
        set.onClick.AddListener(SetButton);
    }

    // Update is called once per frame
    void Update()
    {
        text.text = "H: " + camera.height.ToString() + "W: " + camera.width.ToString() + "Fps: " + camera.updateCount.ToString();
    }

    public void SetButton()
    {
        if (camera.isPlaying) camera.Stop();
        camera = new WebCamTexture();
        plane.GetComponent<Renderer>().material.mainTexture = camera;
        camera.requestedFPS = int.Parse(framerate.text);
        camera.requestedHeight = int.Parse(height.text);
        camera.requestedWidth = int.Parse(width.text);
        camera.Play();
    }
}
