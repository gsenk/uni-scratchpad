function prikaz(ime, sirina, visina, id){
var width = sirina,
    height = visina;

	var x = d3.scale.linear()
		.range([0, width-6]);

	var y = d3.scale.linear()
		.range([0, height - 40]);

	// An SVG element with a bottom-right origin.
	var svg = d3.select("#"+id).append("svg")
		.attr("width", width)
		.attr("height", height)
		.style("padding-right", "30px")
	  .append("g")
		.attr("transform", "translate(" + x(1) + "," + (height - 40) + ")scale(-1,-1)");

	// A sliding container to hold the bars.
	var body = svg.append("g")
		.attr("transform", "translate(0,0)");

	// A container to hold the y-axis rules.
	var rules = svg.append("g");

	// A label for the current year.
	var title = svg.append("text")
	.attr("class", "title")
	.attr("dy", ".71em")
	.attr("transform", "translate(" + x(1) + "," + y(1) + ")scale(-1,-1)")
	.text("");
	
	// A label for mouse hoover.
	var title1 = svg.append("text")
  .attr("class", "title1")
  .attr("dy", "1.01em")
  .attr("transform", "translate(" + x(0.35) + "," + y(1) + ")scale(-1,-1)")
  .text("");
		
	// A label for x axis.
	var xtitle = svg.append("text")
	.attr("class", "xtitle")
	.attr("dy", "1.71em")
	.attr("transform", "translate(" + x(0.5) + "," + y(-0.02) + ")scale(-1,-1)")
	.text("Cena v EUR");

d3.csv(ime, function(data) {

	//Show and hide data on hover
  function showdata(d, i){
    title1.text("Št. artiklov: "+data[year][d]);
  }
  function showdata2(d, i){
    title1.text("");
  }

  // Convert strings to numbers.
  data.forEach(function(d) {
    d.people = +d.people;
    d.year = +d.year;
    d.age = +d.age;
  });
 
  // Compute the extent of the data set in age and years.
  var age0 = 0,
      age1 = d3.max(data, function(d) { return d.age; }),
      year0 = d3.min(data, function(d) { return d.year; }),
      year1 = d3.max(data, function(d) { return d.year; }),
      year = year1;

  // Update the scale domains.
  x.domain([0, age1 + year/2]);
  y.domain([0, d3.max(data, function(d) { return d.people; })]);

  // Add rules to show the population values.
  rules = rules.selectAll(".rule")
      .data(y.ticks(10))
			.enter().append("g")
      .attr("class", "rule")
      .attr("transform", function(d) { return "translate(-25," + y(d) + ")"; });

  rules.append("line")
      .attr("x2", width);
      
  // Add labeled rectangles for each birthyear.
  var years = body.selectAll("g")
      .data(d3.range(year0 - age1, year1 + (year1-year0)/20, (year1-year0)/20))
			.enter().append("g")
      .attr("transform", function(d) { return "translate(" + (x(age1) - x(year0 - d)) + ",0)"; })
      .on("mouseover", showdata)
      .on("mouseout", showdata2);
			
  years.selectAll("rect")
      .data(d3.range(2))
			.enter().append("rect")
      .attr("x", 1)
      .attr("width", (age1-age0)/10)
      .attr("height", 1e-6);

  // Add labels to show the age.
  svg.append("g").selectAll("text")
      .data(d3.range(0, age1 + 5, 5))
			.enter().append("text")
      .attr("text-anchor", "middle")
      .attr("transform", function(d) { return "translate(" + (25+x(age1)-(x(d) + x(5) / 2)) + ",-4)scale(-1,-1)"; })
      .attr("dy", ".71em")
      .text(String);

  // Nest by year then birthyear.
  data = d3.nest()
      .key(function(d) { return d.year; })
      .key(function(d) { return d.year - d.age; })
      .rollup(function(v) { return v.map(function(d) { return d.people; }); })
      .map(data);

  // Allow the arrow keys to change the displayed year.
  d3.select(window).on("keydown", function() {
    switch (d3.event.keyCode) {
      //case 37: {if year=10 year = Math.max(year0, year - 5); else year = Math.max(year0, year - 10); break;
      //case 39: if year=5 year = Math.min(year1, year + 5); else year = Math.min(year1, year + 10); break;
      case 37: year = Math.max(year0, year - 10); break;
      case 39: year = Math.min(year1, year + 10); break;
    }
    redraw();
  });

  redraw();

  function redraw() {
    if (!(year in data)) return;
    title.text(year);

    body.transition()
        .duration(750)
        .attr("transform", function(d) { return "translate(" + (year1 - year)*32 + ",0)"; });

    years.selectAll("rect")
        .data(function(d) { return data[year][d] || [0, 0]; })
      .transition()
        .duration(750)
        .attr("height", y);
  }
});
}