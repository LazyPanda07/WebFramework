using Framework;

public class WebSocketEcho : WebSocketExecutor
{
	public override FramePayload? OnReceive(Frame frame, ref Frame.Close? close)
	{
		return frame.GetPayload();
	}
}
