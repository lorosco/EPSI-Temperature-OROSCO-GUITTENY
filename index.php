<div class="shadow-lg rounded-lg overflow-hidden">
    <div class="py-3 px-5 bg-gray-50">Line chart</div>
    <canvas class="p-10" id="chartLine"></canvas>
    <p>
        <?php
        mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);
        $link =new mysqli("localhost", "epsi", "epsi", "epsi");
        $result = $link->query("SELECT * FROM temperature LIMIT 100");
        $line = "";
        while($obj = $result->fetch_object()){
            $line.=$obj->date;
            $line.=$obj->temperature;
        }
        echo $line;
        $result->close();
        echo "hého";

        echo $result[1];

        $temperature = $result['temperature'];
        $date = $result['date'];

        echo $date;
        echo $temperature;

        ?>
    </p>
</div>

<!-- Required chart.js -->
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

<!-- Chart line -->
<script>
    const labels;
    const data = {
        labels: $date,
        datasets: [{
            label: "Temperature en foncion du temps",
            backgroundColor: "hsl(252, 82.9%, 67.8%)",
            borderColor: "hsl(252, 82.9%, 67.8%)",
            data: $temperature,
        }, ],
    };

    const configLineChart = {
        type: "line",
        data,
        options: {},
    };

    var chartLine = new Chart(
        document.getElementById("chartLine"),
        configLineChart
    );

    function adapterDateTableau(dates) {

    }

    function adaptTempTableau(temperatures) {

    }
</script>