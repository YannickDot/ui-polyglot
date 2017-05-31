module StatefulComponent = {
  include ReactRe.Component.Stateful;
  let name = "StatefulComponent";
  type props = unit;
  type state = { count: int };
  let getInitialState _ => { count: 0 };

  let styling = ReactDOMRe.Style.make
    display::"flex"
    padding::"30px"
    color::"white"
    backgroundColor::"#070A35"
    cursor::"pointer" ();

  let btnStyle = ReactDOMRe.Style.make
    padding::"10px 15px"
    backgroundColor::"green"
    color::"white" ();

  let increment {props, state} event => Some { count: (state.count + 1) };
  let decrement {props, state} event => Some { count: (state.count - 1) };

  let render {props, state, updater, handler, instanceVars, setState} =>
    <div style=(styling)>
      <div>(ReactRe.stringToElement "Hello I'm stateful ! -->")</div>

      <div onClick=(updater increment) style=(btnStyle)>
        (ReactRe.stringToElement "+1")
      </div>
      <div onClick=(updater decrement) style=(btnStyle)>
        (ReactRe.stringToElement "-1")
      </div>
      <div>
        (ReactRe.stringToElement "Count : ")
        (ReactRe.stringToElement (string_of_int state.count))
      </div>
    </div>;
};

include ReactRe.CreateComponent StatefulComponent;

let createElement = wrapProps ();
