<?php
// W1 - "Nettools": a deliberately vulnerable PHP application.
//
// TEACHING NOTE (the inverse of week 6): here the INTERPRETER is patched and fine -
// the bug is in the APPLICATION. This is a classic OS command injection: untrusted
// input is concatenated straight into a shell command line.

$out = '';
if (isset($_GET['host']) && $_GET['host'] !== '') {
    $host = $_GET['host'];
    // VULNERABILITY: $host goes unsanitized into a shell command.
    // The intended use is a hostname/IP; anything the shell understands also runs.
    $out = shell_exec('ping -c 1 -W 1 ' . $host . ' 2>&1');
}
?>
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Nettools - ping</title>
  <style>
    body { font-family: system-ui, sans-serif; max-width: 680px; margin: 3rem auto; padding: 0 1rem; }
    pre  { background:#0f172a; color:#e2e8f0; padding:1rem; border-radius:8px; overflow:auto; }
    input{ padding:.4rem; } button{ padding:.4rem .8rem; }
  </style>
</head>
<body>
  <h1>Nettools</h1>
  <p>Check whether a host is reachable.</p>
  <form method="get">
    <input name="host" placeholder="e.g. 127.0.0.1" size="30" value="<?= isset($_GET['host']) ? htmlspecialchars($_GET['host'], ENT_QUOTES) : '' ?>">
    <button type="submit">Ping</button>
  </form>
  <?php if ($out !== ''): ?>
    <pre><?= htmlspecialchars($out, ENT_QUOTES) ?></pre>
  <?php endif; ?>
</body>
</html>
