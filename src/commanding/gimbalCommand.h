namespace PlaneTracking {
  class GimbalCommand {
    public:
      GimbalCommand(PitchYawDegrees degrees);
    protected:
      PitchYawDegrees degrees;
  };
  class AbsoluteGimbalCommand : GimbalCommand {};
  class RelativeGimbalCommand : GimbalCommand {};
}
