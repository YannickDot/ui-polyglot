module TimerComponent = {
  include ReactRe.Component.Stateful.InstanceVars;
  let name = "TimerComponent";
  type props = { duration: int };
  type state = { count: int };
  type instanceVars = {mutable intervalID: option Js_global.intervalId};

  let getInstanceVars () => {intervalID: None};
  let getInitialState _ => {count: 0};

  let stopTimer instanceVars => {
    switch instanceVars.intervalID {
      | None => ()
      | Some id => (Js_global.clearInterval id)
    };
    instanceVars.intervalID = None;
  };

  let handleStopTimer {instanceVars} _ => {
    stopTimer instanceVars;
    None
  };

  let startTimer {instanceVars, props, setState} _ => {
    instanceVars.intervalID = switch instanceVars.intervalID {
      | None =>
        (Some
          (Js_global.setInterval
            (fun () => {
              setState (fun {state} => {count: state.count + 1});
            })
            props.duration))
      | Some _ => instanceVars.intervalID
    };

    None;
  };

  let componentWillUnmount {instanceVars} => stopTimer instanceVars;

  let styling = ReactDOMRe.Style.make
    padding::"30px"
    color::"white"
    backgroundColor::"#222" ();

  let btnStyle = ReactDOMRe.Style.make
    padding::"10px 15px"
    backgroundColor::"green"
    color::"white" ();

  let render {state, updater} =>
    <div style=(styling)>
      <div>(ReactRe.stringToElement "I'm an interval component ! (using instanceVars - ain't nobody got time for 'this')")</div>
      <button onClick=(updater startTimer)>
        (ReactRe.stringToElement "start")
      </button>
      <button onClick=(updater handleStopTimer)>
        (ReactRe.stringToElement "stop")
      </button>
      <div>
        (ReactRe.stringToElement "Interval count : ")
        (ReactRe.stringToElement (string_of_int state.count))
      </div>
    </div>;
};

include ReactRe.CreateComponent TimerComponent;

let createElement ::duration => wrapProps {duration: duration};
