module StatelessComponent = {
  include ReactRe.Component;
  let name = "StatelessComponent";
  type props = unit;

  let styling = ReactDOMRe.Style.make
    display::"flex"
    padding::"30px"
    color::"white"
    backgroundColor::"#db4d3f"
    cursor::"pointer" ();

  let render {props, updater} =>
    <div style=(styling)>
      (ReactRe.stringToElement "Hello I'm stateless !")
      (ReactRe.stringToElement "")
    </div>;
};

include ReactRe.CreateComponent StatelessComponent;

let createElement = wrapProps ();
