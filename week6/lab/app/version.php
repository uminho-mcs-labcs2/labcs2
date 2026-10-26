<?php
// Minimal diagnostic endpoint for the reconnaissance exercises (E1).
// In production you should NOT expose this — that is part of the class discussion.

header('Content-Type: application/json');

$fixed = [
    '8.5' => '8.5.12',
    '8.4' => '8.4.27',
    '8.3' => '8.3.36',
    '8.2' => '8.2.35',
];

$branch = PHP_MAJOR_VERSION . '.' . PHP_MINOR_VERSION;
$fixedForBranch = $fixed[$branch] ?? null;

$exposed = null;
if ($fixedForBranch !== null) {
    // Version-based assessment for official upstream builds, not an exploitability test.
    $exposed = version_compare(PHP_VERSION, $fixedForBranch, '<');
}

// Teaching caveat: a `null` here means "no fixed release is known for this branch",
// NOT "safe". Older branches lack normal upstream releases; vendor fixes and the
// exceptional 7.4/8.0/8.1 source backports require a separate provenance check.
// Treat `null` as "investigate", never as "PATCHED". Even for a known branch,
// vendor backports cannot be classified reliably using PHP_VERSION alone.
echo json_encode([
    'php_version'       => PHP_VERSION,
    'branch'            => $branch,
    'fixed_release'     => $fixedForBranch,
    'php2shell_exposed' => $exposed,   // true = below fix; false = at/above fix; null = unknown branch
    'note'              => 'Version comparison for upstream builds: CVE-2026-109618 / CVE-2026-109619 only; not an exploitability test or an assessment of CVE-2026-115560.',
], JSON_PRETTY_PRINT) , "\n";
