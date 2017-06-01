module StatefulComponent = {
  include ReactRe.Component.Stateful;
  let name = "StatefulComponent";
  type props = unit;
  type state = { count: int };
  let getInitialState _ => { count: 0 };

  let styling = ReactDOMRe.Style.make
    padding::"30px"
    color::"white"
    backgroundColor::"#070A35" ();

  let increment {state} _ => Some { count: (state.count + 1) };
  let decrement {state} _ => Some { count: (state.count - 1) };

  let render {state, updater} =>
    <div style=(styling)>
      <div>(ReactRe.stringToElement "I'm a stateful component ! ")</div>

      <button onClick=(updater increment)>
        (ReactRe.stringToElement "+1")
      </button>
      <button onClick=(updater decrement)>
        (ReactRe.stringToElement "-1")
      </button>
      <div>
        (ReactRe.stringToElement
          ("Count : " ^ (string_of_int state.count)))
      </div>
    </div>;
};

include ReactRe.CreateComponent StatefulComponent;

let createElement = wrapProps ();
