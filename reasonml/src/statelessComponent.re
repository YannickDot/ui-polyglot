module StatelessComponent = {
  include ReactRe.Component;
  let name = "StatelessComponent";
  type props = { name: string };

  let styling = ReactDOMRe.Style.make
    padding::"30px"
    color::"white"
    backgroundColor::"#db4d3f" ();

  let render {props, updater} =>
    <div style=(styling)>
      (ReactRe.stringToElement
        ("I'm a stateless component ! And my name is " ^ props.name))
    </div>;
};

include ReactRe.CreateComponent StatelessComponent;

let createElement ::name => wrapProps {name: name};
