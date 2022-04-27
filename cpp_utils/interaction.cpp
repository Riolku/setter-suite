template <typename Parent> class InteractorReader : public Parent {
protected:
  void preError() override { preErrorHook(); }

public:
  using Parent::Parent;

  // close stdout before reading EOF.
  ~InteractorReader() { fclose(stdout); }
};
