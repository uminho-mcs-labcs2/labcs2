<?php
// W2 - "Preferences": a deliberately vulnerable PHP application.
//
// TEACHING NOTE: this demonstrates PHP Object Injection. The app stores a user's
// display preferences as a serialized PHP object in the `prefs` query parameter and
// restores them with unserialize() on an untrusted value. If an attacker supplies a
// serialized object of a class whose "magic method" does something dangerous, that
// code runs when the object is created/destroyed. Such a class is called a "gadget".

// --- A legitimate class the app uses for its intended feature ---------------------
class Preferences {
    public $theme = 'light';
    public $lang  = 'en';
    public function describe() {
        return "theme={$this->theme}, lang={$this->lang}";
    }
}

// --- An unrelated utility class that happens to be loadable ------------------------
// Its __destruct runs automatically when the object is cleaned up at end of request.
// VULNERABILITY: it reveals the contents of any file named in $path. Combined with
// unserialize() of attacker input, this becomes an arbitrary file read (a "gadget").
class Cache {
    public $path = '';
    public function __destruct() {
        if ($this->path !== '') {
            echo "\n<!-- cache dump -->\n" . file_get_contents($this->path);
        }
    }
}

$prefsObj = new Preferences();
$note = 'Using default preferences.';

if (isset($_GET['prefs']) && $_GET['prefs'] !== '') {
    // VULNERABILITY: unserialize() on untrusted, attacker-controlled input.
    $decoded = base64_decode($_GET['prefs'], true);
    if ($decoded !== false) {
        $maybe = unserialize($decoded);
        if ($maybe instanceof Preferences) {
            $prefsObj = $maybe;
            $note = 'Loaded your saved preferences.';
        } else {
            $note = 'Preferences restored.';
        }
    }
}

// So students can see the expected format, show what a benign blob looks like.
$example = base64_encode(serialize(new Preferences()));
?>
<!doctype html>
<html lang="<?= htmlspecialchars($prefsObj->lang, ENT_QUOTES) ?>">
<head>
  <meta charset="utf-8">
  <title>Preferences</title>
  <style>
    body { font-family: system-ui, sans-serif; max-width: 680px; margin: 3rem auto; padding: 0 1rem; }
    code { background:#eef; padding:0 .25rem; border-radius:3px; word-break:break-all; }
  </style>
</head>
<body>
  <h1>Your preferences</h1>
  <p><?= htmlspecialchars($note, ENT_QUOTES) ?></p>
  <p>Current: <strong><?= htmlspecialchars($prefsObj->describe(), ENT_QUOTES) ?></strong></p>
  <hr>
  <p>Preferences are carried in the <code>prefs</code> parameter (base64 of a serialized
     object). For example, your current settings encode to:</p>
  <p><code><?= htmlspecialchars($example, ENT_QUOTES) ?></code></p>
  <p>Try: <code>?prefs=&lt;blob&gt;</code></p>
</body>
</html>
