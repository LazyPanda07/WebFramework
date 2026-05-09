namespace Framework;

public abstract class WebSocketExecutor
{
	public abstract FramePayload? OnReceive(Frame frame, ref Frame.Close? close);
}
