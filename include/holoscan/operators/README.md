# Operators

## Folder Structure

It currently includes Operators (components) that are needed to create the Endoscopy Sample application.

- custom_lstm_inference
- format_converter
- stream_playback
  - This folder has both VideoStreamReplayerOp and VideoStreamRecorderOp
  - Good candidates to build a GXF extension that has multiple Operators
    - VideoStreamReplayerOp is using Holoscan API and VideoStreamRecorderOp is generated by a GXF extension
- tensor_rt
  - This folder has GXF's built-in TensorRT Codelet(Operator).
- visualizer_tool_tracking
