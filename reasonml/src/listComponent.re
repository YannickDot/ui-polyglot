module ListComponent = {
  include ReactRe.Component.Stateful;
  let name = "ListComponent";
  type props = unit;
  type state = { numbers: list int };
  let getInitialState _ => { numbers: [0, 1, 2] };

  let styling = ReactDOMRe.Style.make
    padding::"30px"
    color::"pink"
    backgroundColor::"papayawhip" ();

  let renderListItem item => {
    <li
      key={string_of_int item}>
      (ReactRe.stringToElement (string_of_int item))
    </li>
  };

  let render {state} =>
    <div style=(styling)>
      <div>(ReactRe.stringToElement "I'm a list component ! ")</div>
      <ul>
        (ReactRe.listToElement (List.map renderListItem state.numbers))
      </ul>
    </div>;
};

include ReactRe.CreateComponent ListComponent;

let createElement = wrapProps ();
