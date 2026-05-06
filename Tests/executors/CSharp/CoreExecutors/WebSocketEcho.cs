using Framework;

public class WebSocketEcho : WebSocketExecutor
{
	public override FramePayload? OnReceive(Frame frame)
	{
		return frame.GetPayload();
	}
}
