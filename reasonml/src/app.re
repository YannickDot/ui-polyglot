module App = {
  include ReactRe.Component;
  /* The props type */
  type props = {title: string};
  /* The string name of the component */
  let name = "App";
  /*  */
  let handleClick _ _ => {
    Js.log "clicked!";
    None
  };
  /* {props, state, updater, handler, instanceVars, setState} */
  let styling = ReactDOMRe.Style.make
    padding::"30px"
    backgroundColor::"#db4d3f" ();

  let titleStyle = ReactDOMRe.Style.make
    fontSize::"2em" ();

  let render {props, updater} =>
    <div className="App">
      <div className="App-header">
        <div
          onClick=(updater handleClick)>
        </div>
        <h2 style=(titleStyle)>
          (ReactRe.stringToElement props.title)
        </h2>
      </div>
      <div className="App-intro">
        <StatelessComponent name="Sam"/>
        <StatefulComponent />
        <TimerComponent duration={100}/>
        <FetchComponent url="http://"/>
        <ListComponent />
      </div>
    </div>;
};

include ReactRe.CreateComponent App;

let createElement ::title => wrapProps {title: title};
