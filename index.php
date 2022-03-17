<html>

<div class="shadow-lg rounded-lg overflow-hidden">
    <div class="py-3 px-5 bg-gray-50">Line chart</div>
    <canvas class="p-10" id="chartLine"></canvas>
    <?php
    $date = array();
    $temperature = array();
    function getData($limit)
    {
        mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);
        $link = new mysqli("localhost", "epsi", "epsi", "epsi");
        $result = $link->query("SELECT * FROM temperature ORDER BY date desc LIMIT $limit");

        while ($obj = $result->fetch_object()) {
            $date[] = $obj->date;
            $temperature[] = $obj->temperature;
        }
        $result->close();
    }
    ?>
</div>

<!-- Required chart.js -->
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

<!-- Chart line -->
<script>
    let canva = document.getElementById("chartLine");

    let context = canva.getContext("2d");

    function updateData() {
        <?php getData(20) ?>
        context.clearRect(0, 0, canva.width, canva.height);
        const labels = <?php $data = "[";
                        foreach (explode(",", implode(",", $date)) as $val) {
                            $data .= "'";
                            $data .= $val;
                            $data .= "',";
                        }

                        substr($data, 0, -1);
                        $data .= "]";
                        echo $data ?>;
        const data = {
            labels: labels,
            datasets: [{
                label: "Temperature en foncion du temps",
                backgroundColor: "hsl(252, 82.9%, 67.8%)",
                borderColor: "hsl(252, 82.9%, 67.8%)",
                data: <?php $data = "[";
                        foreach (explode(",", implode(",", $temperature)) as $val) {
                            $data .= $val;
                            $data .= ",";
                        }
                        substr($data, 0, -1);
                        $data .= "]";
                        echo $data ?>,
            }, ],
        };
    }
    setInterval(updateData(),
        11000);

    updateData();

    const configLineChart = {
        type: "line",
        data,
        options: {},
    };
    var chartLine = new Chart(
        document.getElementById("chartLine"),
        configLineChart
    );
</script>

</html>