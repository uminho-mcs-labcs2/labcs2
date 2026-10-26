<?php
// Small teaching application, with no deliberately introduced application exploit.
// Runtime vulnerabilities can affect an app independently of its input validation.
//
// A small in-memory "wall", just so there is a representative app running.

session_start();
if (!isset($_SESSION['wall'])) {
    $_SESSION['wall'] = [];
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['msg'])) {
    // Note: htmlspecialchars() escapes HTML so user text can't inject script,
    // preventing cross-site scripting (XSS) — the app does its job correctly.
    $msg = trim((string) $_POST['msg']);
    if ($msg !== '') {
        $_SESSION['wall'][] = htmlspecialchars($msg, ENT_QUOTES, 'UTF-8');
    }
    // Redirect to a constant target (post/redirect/get), rather than using an
    // attacker-influenced request path as a redirect target.
    header('Location: index.php');
    exit;
}
?>
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>LabCS2 — Wall (week 6)</title>
  <style>
    body { font-family: system-ui, sans-serif; max-width: 640px; margin: 3rem auto; padding: 0 1rem; }
    .banner { background:#f3f4f6; border-left:4px solid #6366f1; padding:.75rem 1rem; border-radius:6px; }
    li { margin:.25rem 0; }
    code { background:#eef; padding:0 .25rem; border-radius:3px; }
  </style>
</head>
<body>
  <h1>Wall — LabCS2</h1>
  <p class="banner">
    Teaching application. This session studies vulnerabilities in the
    <strong>PHP interpreter</strong> that runs it
    (<code><?= htmlspecialchars(PHP_VERSION) ?></code>).
  </p>

  <form method="post">
    <input name="msg" placeholder="Write something…" autocomplete="off" size="40">
    <button type="submit">Post</button>
  </form>

  <ul>
    <?php foreach (array_reverse($_SESSION['wall']) as $m): ?>
      <li><?= $m ?></li>
    <?php endforeach; ?>
  </ul>

  <hr>
  <p><small>Diagnostic endpoint: <a href="version.php">version.php</a></small></p>
</body>
</html>
